<?php
require_once 'NDB_Menu_Filter.class.inc';

/**
* The candidate list menu
 * @package main
*/
class NDB_Menu_Filter_candidate_list extends NDB_Menu_Filter
{
    /**
    * overloading this method to allow access to site users (their own site only) and users w/ multisite privs
    * @note   overloaded function
    * @return bool
    * @access private
    */
    function _hasAccess()
    {
        // create user object
        $user =& User::singleton();
        if(PEAR::isError($user)) {
            return PEAR::raiseError("User Error: ".$user->getMessage());
        }

        // allow only to view own site data
        $site =& Site::singleton($user->getData('CenterID'));
        if (PEAR::isError($site)) {
            return PEAR::raiseError("DB Error: ".$site->getMessage());
        }
        
        return ($user->hasPermission('access_all_profiles') ||  $site->isStudySite());
    }

    function _setupVariables()
    {
        // set the class variables
        $this->columns = array('psc.Name AS PSC', 'candidate.CandID AS DCCID', 'candidate.PSCID', 'candidate.Gender', 'min(session.SubprojectID) as Subproject', 'DATE_FORMAT(candidate.DoB,\'%Y-%m-%d\') AS DoB');
        $config=&NDB_Config::singleton();
        if($config->getSetting("useEDC")=="true"){
        	$this->columns[]='DATE_FORMAT((candidate.EDC),\'%Y-%m-%d\') AS EDC';
        	$this->formToFilter['edc'] = 'candidate.EDC';
        	$this->validFilters[]='candidate.EDC';
        }
        $this->columns=array_merge($this->columns,array('max(session.VisitNo) as Visit_count', 'max(session.Current_stage) as Latest_visit_status', 'min(feedback_bvl_thread.Status+0) as Feedback'));

        
        $this->query = " FROM psc, candidate LEFT JOIN session ON candidate.CandID = session.CandID AND session.Cancelled = 'N' AND session.Active = 'Y'
         LEFT JOIN feedback_bvl_thread ON (candidate.CandID=feedback_bvl_thread.CandID)
        WHERE candidate.CenterID = psc.CenterID AND candidate.Entity_type = 'Human' AND candidate.Cancelled = 'N' AND candidate.Active = 'Y' ";
        
        $this->group_by = 'candidate.CandID';
        $this->order_by = 'psc.Name, candidate.CandID DESC, session.VisitNo';
        $this->validFilters = array('candidate.CenterID', 'candidate.CandID', 'candidate.PSCID', 'candidate.Gender', 'session.SubprojectID', 'candidate.DoB');
		
        $this->formToFilter = array(
                                    'centerID' => 'candidate.CenterID',
                                    'DCCID' => 'candidate.CandID',
                                    'PSCID' => 'candidate.PSCID',
                                    'gender' => 'candidate.Gender',
                                    'SubprojectID' => 'session.SubprojectID',
                                    'dob' => 'candidate.DoB',
                                    );
        return true;
    }


    function _setFilterForm()
    {
        // create user object
        $user =& User::singleton();
        if(PEAR::isError($user)) {
            return PEAR::raiseError("User Error: ".$user->getMessage());
        }

        // PSC
        if ($user->hasPermission('access_all_profiles')) {
            // get the list of study sites - to be replaced by the Site object
            $list_of_sites =& Utility::getSiteList();
            if(PEAR::isError($list_of_sites)) {
                return PEAR::raiseError("DB Error: ".$list_of_sites->getMessage());
            }
            if(is_array($list_of_sites)) $list_of_sites = array('' => 'Any') + $list_of_sites;
        }
        else {
            // allow only to view own site data
            $site =& Site::singleton($user->getData('CenterID'));
            if (PEAR::isError($site)) {
                return PEAR::raiseError("DB Error: ".$site->getMessage());
            }
            if ($site->isStudySite()) {
                $list_of_sites = array($user->getData('CenterID') => $user->getData('Site'));
            }
        }

        // SubprojectID
        $list_of_subprojects =& Utility::getSubprojectList();

        // list of feedback statuses
        $feedback_status_options = array('opened'=>'opened','answered'=>'answered','closed'=>'closed','comment'=>'comment');
    
        // add form elements
        $this->form->addElement('select', 'centerID', 'Site:', $list_of_sites);
        $this->form->addElement('text', 'DCCID', 'DCC-ID:');
        $this->form->addElement('text', 'PSCID', 'PSC-ID:');
        $this->form->addElement('select', 'gender', 'Gender:', array('' => 'Any', 'Male' => 'Male', 'Female' => 'Female'));
        $this->form->addElement('select', 'SubprojectID', 'Sub Project:', array('' => 'Any') + $list_of_subprojects);
        $this->form->addElement('text', 'dob', 'Date of Birth:', 'maxlength=10');
        
        $config=&NDB_Config::singleton();
        if($config->getSetting("useEDC")=="true"){
        	$this->form->addElement('text', 'edc', 'EDC:', 'maxlength=10');
        	$this->tpl_data['useEDC']="true";
        }
        $this->form->addElement('select', 'feedback', 'Feedback Status:', array_merge(array('' => 'Any'), $feedback_status_options));

        return true;
    }


    function _setDataTableRows($count)
    {
        // create user object
        $user =& User::singleton();
        if(PEAR::isError($user)) {
            return PEAR::raiseError("User Error: ".$user->getMessage());
        }
		$subprojectlist=Utility::getSubprojectList();
		
        // print out
        $x = 0;
        foreach ($this->list as $item) {
            //count column
            $this->tpl_data['items'][$x][0]['value'] = $x + $count;

            //print out data rows
            $i = 1;
            foreach ($item as $key => $val) {
                //if it's a non-PSC user show the URL to the timepoint list on PSCID field
                if ($key == 'PSCID' && $user->hasPermission('access_all_profiles')) {
                    $this->tpl_data['items'][$x][$i]['DCCID'] = $item['DCCID'];
                }
                $this->tpl_data['items'][$x][$i]['name'] = $key;
                $this->tpl_data['items'][$x][$i]['value'] = $val;
                // get feedback information for each displayed candidate record
                if ($key == 'Feedback' && !empty($val)) {
                    // overwrite value
                    $this->tpl_data['items'][$x][$i]['value'] = NDB_BVL_Feedback::getFeedbackStatusVerbose($val);
                    $this->tpl_data['items'][$x][$i]['bgcolor'] = NDB_BVL_Feedback::getThreadColor($this->tpl_data['items'][$x][$i]['value']);
                } else if ($key=='Feedback' && empty($val)){
                	$this->tpl_data['items'][$x][$i]['value'] = "None";
                } else if ($key=='Subproject'){
                	$this->tpl_data['items'][$x][$i]['value'] = $subprojectlist[$val];
                }
                	

                $i++;
            }

            $x++;
        }

        return true;
    }
}
?>