<?php
require_once "NDB_Form.class.inc";

/**
 * The forms for the new profile menu
 * @package main
 */
class NDB_Form_new_profile extends NDB_Form
{
    function _hasAccess()
    {
        // create user object
        $user =& User::singleton();
        if(PEAR::isError($user)) {
            return PEAR::raiseError("User Error: ".$user->getMessage());
        }

        $site =& Site::singleton($user->getData('CenterID'));
        if (PEAR::isError($site)) {
            return PEAR::raiseError("Unable to construct the list_of_sites array: ".$site->getMessage());
        }
        if ($site->isStudySite()) {
            return $user->hasPermission('data_entry');
        }

        return false;
    }

    function _process($values)
    {
        // set up the arguments to Candidate::createNew
        $user =& User::singleton();
        $config =& NDB_Config::singleton();
        $dob = sprintf("%04d-%02d-%02d", $values['dob1']['Y'], $values['dob1']['M'], $values['dob1']['d']);
        
        if($config->getSetting('useEDC') == "true"){
            $edc = sprintf("%04d-%02d-%02d", $values['edc1']['Y'], $values['edc1']['M'], $values['edc1']['d']);
            if($edc == '0000-00-00') {
                $edc = NULL;
            }
        }

        // create the candidate
        $candID = Candidate::createNew($user->getData('CenterID'), $dob, $edc, $values['gender'], $values['PSCID']);
        if (PEAR::isError($candID)) {
            return PEAR::raiseError("new_profile::_process(): ".$candID->getMessage());
        }

        // get the candidate
        $candidate =& Candidate::singleton($candID);
        if (PEAR::isError($candidate)) {
            return PEAR::raiseError("Candidate Error ($candID): ".$candidate->getMessage());
        }

        //------------------------------------------------------------

        $this->tpl_data['success'] = true;
        $this->tpl_data['candID'] = $candID;
        $this->tpl_data['PSCID'] = $candidate->getPSCID();

        // freeze it, just in case
        $this->form->freeze();
    }

    function new_profile()
    {
        $config =& NDB_Config::singleton();
        $study = $config->getSetting('study');
        $dateOptions = array(
                             'language'        => 'en',
                             'format'          => 'YMd',
                             'addEmptyOption'  => true,
                             'minYear'         => $study['startYear'] - $study['ageMax'],
                             'maxYear'         => $study['endYear'] - $study['ageMin']
                             );

        // add date of birth
        $this->form->addElement('date', 'dob1', 'Date of Birth', $dateOptions);
        $this->form->addElement('date', 'dob2', 'Confirm Date of Birth', $dateOptions);
    
        // date of birth rules
        $this->form->addRule('dob1', 'Date of Birth is required', 'required');
        $this->form->addRule(array('dob1', 'dob2'), 'Date of Birth fields must match', 'compare');

        if($config->getSetting('useEDC') == "true"){
            // add expected date of confinement
            $this->form->addElement('date', 'edc1', 'Expected Date of Confinement', $dateOptions);
            $this->form->addElement('date', 'edc2', 'Confirm EDC', $dateOptions);

            // expected date of confinement rules
            $this->form->addRule(array('edc1', 'edc2'), 'EDC fields must match', 'compare');
        }

        // add gender
        $genderOptions = array('', 'Male' => 'Male', 'Female' => 'Female');
        $this->form->addElement('select', 'gender', 'Gender', $genderOptions);
    
        // gender rules
        $this->form->addRule('gender', 'Gender is required', 'required');

        $PSCIDsettings = $config->getSetting('PSCID');
        if($PSCIDsettings['generation'] == 'user') {
            $this->form->addElement('text', 'PSCID', 'PSCID');
        }

        $this->form->addFormRule(array(&$this, '_validate'));
    }

    function _validate($values)
    {
        $errors = array();

        $config =& NDB_Config::singleton();
        // check date pairs
        if (empty($values['dob1']['M']) && empty($values['dob1']['d']) && empty($values['dob1']['Y'])) {
            $errors['dob1'] .= "Date is required. \n";
        }
        elseif (!_checkDate($values['dob1'])) {
            $errors['dob1'] = 'Date of Birth is not a valid date';
        }
    
        if($config->getSetting('useEDC') == "true") {
            if (strlen(implode($values['edc1'])) > 2 && !_checkDate($values['edc1'])) {
                $errors['edc1'] = 'EDC is not a valid date';
            }
        }

        if (empty($values['gender'])) {
            $errors['gender'] = 'Gender is required';
        }
    
        $PSCIDsettings = $config->getSetting('PSCID');
        if($PSCIDsettings['generation'] == 'user') {
            $db =& Database::singleton();
            $user =& User::singleton();
            $centerID = $user->getData('CenterID');
            $site =& Site::singleton($centerID);

            if(empty($values['PSCID'])) $errors['PSCID'] = 'PSCID must be specified';
            elseif(!Candidate::validatePSCID($values['PSCID'], $site->getSiteAlias())) $errors['PSCID'] = 'PSCID does not match the required structure';
            elseif($db->selectOne("SELECT count(PSCID) FROM candidate WHERE PSCID='$values[PSCID]'") > 0) $errors['PSCID'] = 'PSCID has already been registered';
        }

        return $errors;
    }

}
?>