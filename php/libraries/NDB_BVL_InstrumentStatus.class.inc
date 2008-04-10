<?
/**
 * Behavioural instrument status class
 *
 * This class provides management tools for the status flags of instruments
 * in the NeuroDB framework.
 * @package behavioural  
 * @access public
 */
class NDB_BVL_InstrumentStatus
{
    /**
     * instrument instance CommentID
     * @access private
     */
    var $_commentID;

    /**
     * status array
     * @access private
     */
    var $_status;

    /**
     * the set of valid options for the data entry flag
     * @access private
     */
    var $_dataEntryOptions = array(null, 'In Progress', 'Complete');

    /**
     * the set of valid options for the administration flag
     * @access private
     */
    var $_administrationOptions = array(null, 'None', 'Partial', 'All');

    /**
     * the set of valid options for the validity flag
     * @access private
     */
    var $_validityOptions = array(null, 'Questionable', 'Invalid', 'Valid');

    /**
     * the set of valid options for the exclusion flag
     * @access private
     */
    var $_exclusionOptions = array(null, 'Fail', 'Pass');


    /**
     * loads the object with the current status of the instrument
     *
     * @param string $commentID the CommentID identifying the data to load
     * @return void
     * @throws PEAR::Error
     * @access public
     */
    function select($commentID)
    {
        // set the _commentID property
        $this->_commentID = $commentID;

        // make sure a Database object exists
        if(!isset($GLOBALS['DB'])) {
            return PEAR::raiseError("No Database object found!");
        }
        
        // make a local reference to the Database object
        $db =& $GLOBALS['DB'];

        // get candidate data from database
        $query = "SELECT SessionID, Data_entry, Administration, Validity, Exclusion FROM flag WHERE CommentID='$this->_commentID'";
        $db->selectRow($query, $row);
        
        if(PEAR::isError($row)) {
            print $row->getMessage();
            return PEAR::raiseError("Could not retrieve instrument status data from database");
        }

        // store the statuses into the _status property
        $this->_status = $row;
    } // end function select()

    /**
    * gets the SessionID of the current instrument
    * @return int
    * @access public
    */
    function getSessionID()
    {
        return $this->_status['SessionID'];
    }
    
    /**
     * gets the current data entry status of the instrument
     * @return string
     * @access public
     */
    function getDataEntryStatus()
    {
        return $this->_status['Data_entry'];
    }

    /**
     * gets the current administration status of the instrument
     * @return string
     * @access public
     */
    function getAdministrationStatus()
    {
        return $this->_status['Administration'];
    }

    /**
     * gets the current validity status of the instrument
     * @return string
     * @access public
     */
    function getValidityStatus()
    {
        return $this->_status['Validity'];
    }

    /**
     * gets the current exclusion status of the instrument
     * @return string
     * @access public
     */
    function getExclusionStatus()
    {
        return $this->_status['Exclusion'];
    }

    /**
     * sets the current data entry status of the instrument
     * @param string $status  the new status from the set (NULL, 'In Progress','Not Complete','Complete')
     * @return void
     * @throws PEAR::Error
     * @access public
     */
    function setDataEntryStatus($status)
    {
        if(!isset($this->_commentID)) {
            return PEAR::raiseError("No instrument instance selected");
        }

        if(!in_array($status, $this->_dataEntryOptions)) {
            return PEAR::raiseError('Invalid data entry status');
        }

        $GLOBALS['DB']->update('flag', array('Data_entry'=>$status), array('CommentID'=>$this->_commentID));
        $updateResult = $this->select($this->_commentID);
        if(PEAR::isError($updateResult)) {
            return PEAR::raiseError("Could not reload object data");
        }
    }

    /**
     * sets the current administration status of the instrument
     * @param string $status  the new status from the set (NULL, 'None','Partial','All')
     * @return void
     * @throws PEAR::Error
     * @access public
     */
    function setAdministrationStatus($status)
    {
        if(!isset($this->_commentID)) {
            return PEAR::raiseError("No instrument instance selected");
        }

        if(!in_array($status, $this->_administrationOptions)) {
            return PEAR::raiseError('Invalid administration status');
        }

        $GLOBALS['DB']->update('flag', array('Administration'=>$status), array('CommentID'=>$this->_commentID));
        $updateResult = $this->select($this->_commentID);
        if(PEAR::isError($updateResult)) {
            return PEAR::raiseError("Could not reload object data");
        }
    }

    /**
     * sets the current validity status of the instrument
     * @param string $status  the new status from the set (NULL, 'Questionable', 'Invalid', 'Valid')
     * @return void
     * @throws PEAR::Error
     * @access public
     */
    function setValidityStatus($status)
    {
        if(!isset($this->_commentID)) {
            return PEAR::raiseError("No instrument instance selected");
        }

        if(!in_array($status, $this->_validityOptions)) {
            return PEAR::raiseError('Invalid administration status');
        }

        $GLOBALS['DB']->update('flag', array('Administration'=>$status), array('CommentID'=>$this->_commentID));
        $updateResult = $this->select($this->_commentID);
        if(PEAR::isError($updateResult)) {
            return PEAR::raiseError("Could not reload object data");
        }
    }

    /**
     * sets the current exclusion status of the instrument
     * @param string $status  the new status from the set (NULL, 'Fail', 'Pass')
     * @return void
     * @throws PEAR::Error
     * @access public
     */
    function setExclusionStatus($status)
    {
        if(!isset($this->_commentID)) {
            return PEAR::raiseError("No instrument instance selected");
        }

        if(!in_array($status, $this->_exclusionOptions)) {
            return PEAR::raiseError('Invalid exclusion status');
        }

        $GLOBALS['DB']->update('flag', array('Exclusion'=>$status), array('CommentID'=>$this->_commentID));
        $updateResult = $this->select($this->_commentID);
        if(PEAR::isError($updateResult)) {
            return PEAR::raiseError("Could not reload object data");
        }
    }

} // end class