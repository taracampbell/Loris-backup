<?
/**
 * @package mri
 */
class MRIFile
{
    var $fileData = array();
    var $parameters = array();
    
    function MRIFile($fileID)
    {
        $db =& Database::singleton();
        
        $query = "SELECT * FROM files WHERE FileID=$fileID";
        $db->selectRow($query, $fileData);
        foreach($fileData AS $key=>$value) {
            $this->fileData[$key] = $value;
        }
        
        $query = "SELECT Name, Value FROM parameter_file as p, parameter_type as f WHERE f.ParameterTypeID=p.ParameterTypeID AND FileID=$fileID";
        $db->select($query, $parameterRaw);
        foreach($parameterRaw AS $row) {
            $this->parameters[$row['Name']] = $row['Value'];
        }
    }
    
    function getParameter($parameterName)
    {
        if(isset($this->fileData[$parameterName]))       return $this->fileData[$parameterName];
        elseif(isset($this->parameters[$parameterName])) return $this->parameters[$parameterName];
        else return null;
    }
    
    function getAcquisitionProtocol()
    {
        $db =& Database::singleton();
        
        if(is_null($this->fileData['AcquisitionProtocolID'])) return null;
        
        $query = "SELECT Scan_type FROM mri_scan_type WHERE ID='".$this->fileData['AcquisitionProtocolID']."'";
        $acquisitionProtocol = $db->selectOne($query);
        return $acquisitionProtocol;
    }
}