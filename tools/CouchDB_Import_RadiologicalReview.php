<?php
require_once 'generic_includes.php';
require_once 'CouchDB.class.inc';
require_once 'Database.class.inc';
class CouchDBRadiologicalReviewImporter {
    var $SQLDB; // reference to the database handler, store here instead
                // of using Database::singleton in case it's a mock.
    var $CouchDB; // reference to the CouchDB database handler

    var $Dictionary = array(
        'CandID' => array(
            'Description' => 'DCC Candidate Identifier',
            'Type' => 'varchar(255)'
        ),  
        'PSCID' => array(
            'Description' => 'Project Candidate Identifier',
            'Type' => 'varchar(255)'
        ),  
        'Visit_label' => array(
            'Description' => 'Visit of Candidate',
            'Type' => 'varchar(255)'
        ),
        'FinalReview_Radiologist' => array(
            'Description' => 'Radiologist/Reviewer doing the final review',
            'Type' => 'varchar(255)'
        ),  
        'FinalReview_Done' => array(
            'Description' => 'Final review done',
            'Type' => 'tinyint(1)'
        ),  
        'FinalReview_Results' => array(
            'Description' => 'Results of the final radiology review',
            'Type' => "enum('normal','abnormal','atypical','not_answered')"
        ),  
        'FinalReview_ExclusionaryStatus' => array(
            'Description' => 'Final review exclusionary status',
            'Type' => "enum('exclusionary','non_exclusionary','not_answered')"
        ),  
        'FinalReview_SAS' => array(
            'Description' => 'Final review subarachnoid space',
            'Type' => 'int(11)'
        ),  
        'FinalReview_PVS' => array(
            'Description' => 'Final review perivascular space',
            'Type' => 'int(11)',
        ),  
        'FinalReview_Comment' => array(
            'Description' => 'Current stage of visit',
            'Type' => 'text'
        ),  
        'FinalReview_Finalized' =>  array(
            'Description' => 'Whether Recycling Bin Candidate was failure or withdrawal',
            'Type' => "tinyint(1)",
        ),
        'ExtraReview_Radiologist' => array(
            'Description' => 'Radiologist/Reviewer doing the extra review',
            'Type' => 'varchar(255)'
        ),  
        'ExtraReview_Done' => array(
            'Description' => 'Extra review done',
            'Type' => 'tinyint(1)'
        ),  
        'ExtraReview_Results' => array(
            'Description' => 'Results of the extra radiology review',
            'Type' => "enum('normal','abnormal','atypical','not_answered')"
        ),  
        'ExtraReview_ExclusionaryStatus' => array(
            'Description' => 'Extra review exclusionary status',
            'Type' => "enum('exclusionary','non_exclusionary','not_answered')"
        ),  
        'ExtraReview_SAS' => array(
            'Description' => 'Extra review subarachnoid space',
            'Type' => 'int(11)'
        ),  
        'ExtraReview_PVS' => array(
            'Description' => 'Extra review perivascular space',
            'Type' => 'int(11)',
        ),  
        'ExtraReview_Comment' => array(
            'Description' => 'Current stage of visit',
            'Type' => "enum('Not Started','Screening','Visit','Approval','Subject','Recycling Bin')"
        ),  
        'SiteReview_Radiologist' => array(
            'Description' => 'Radiologist/Reviewer doing the final review',
            'Type' => 'varchar(255)'
        ),  
        'SiteReview_Done' => array(
            'Description' => 'Project Candidate Identifier',
            'Type' => 'varchar(255)'
        ),  
        'SiteReview_Results' => array(
            'Description' => 'Results of the final radiology review',
            'Type' => 'varchar(255)'
        ),  
        'SiteReview_ExclusionaryStatus' => array(
            'Description' => 'Final review exclusionary status',
            'Type' => 'varchar(255)'
        ),   
        'SiteReview_Comment' => array(
            'Description' => 'Current stage of visit',
            'Type' => "enum('Not Started','Screening','Visit','Approval','Subject','Recycling Bin')"
        )
    );

    function __construct() {
        $this->SQLDB = Database::singleton();
        $this->CouchDB = CouchDB::singleton();
    }

    function run() {

        $this->CouchDB->replaceDoc('DataDictionary:RadiologicalReview',
            array('Meta' => array('DataDict' => true),
                  'DataDictionary' => array('RadiologicalReview' => $this->Dictionary) 
            )
        );
        
        $radiologicalreview = $this->SQLDB->pselect("SELECT c.CandID, c.PSCID, s.Visit_label,
            eFinal.full_name AS FinalReview_Radiologist, frr.Review_done AS FinalReview_Done, 
            frr.Final_Review_Results AS FinalReview_Results, 
            frr.Final_Exclusionary AS FinalReview_ExclusionaryStatus, frr.SAS AS FinalReview_SAS, 
            frr.PVS AS FinalReview_PVS, frr.Final_Incidental_Findings AS FinalReview_Comment, 
            eExtra.full_name AS ExtraReview_Radiologist, frr.Review_Done2 AS ExtraReview_Done, 
            frr.Final_Review_Results2 AS ExtraReview_Results, frr.SAS2 AS ExtraReview_SAS, 
            frr.PVS2 AS ExtraReview_PVS, frr.Final_Incidental_Findings2 AS ExtraReview_Comment, 
            eSite.full_name AS SiteReview_Radiologist, rr.Scan_done AS SiteReview_Done, 
            rr.Review_results AS SiteReview_Results, 
            rr.abnormal_atypical_exculsionary AS SiteReview_ExclusionaryStatus, 
            rr.Incidental_findings AS SiteReview_Comment
            FROM final_radiological_review frr
            LEFT JOIN flag f ON (f.CommentID=frr.CommentID) 
            LEFT JOIN session s ON (s.ID=f.SessionID) 
            LEFT JOIN candidate c ON (c.CandID=s.CandID)
            LEFT JOIN radiology_review rr ON (rr.CommentID=f.CommentID)
            LEFT JOIN examiners eFinal ON (eFinal.ExaminerID=frr.Final_Examiner)
            LEFT JOIN examiners eExtra ON (eExtra.ExaminerID=frr.Final_Examiner2)
            LEFT JOIN examiners eSite ON (eSite.ExaminerID=rr.Examiner)", array());
        foreach($radiologicalreview as $review) {
            $id = 'Radiological_Review_' . $radiologicalreview['PSCID'] . '_' . $radiologicalreview['Visit_label'];
            $success = $this->CouchDB->replaceDoc($id, array('Meta' => array(
                'DocType' => 'RadiologicalReview',
                'identifier' => array($radiologicalreview['PSCID'], $radiologicalreview['Visit_label'])
            ),
                'data' => $radiologicalreview
            ));
            print "$id: $success\n";
        }
    }
}

// Don't run if we're doing the unit tests; the unit test will call run.
if(!class_exists('UnitTestCase')) {
    $Runner = new CouchDBRadiologicalReviewImporter();
    $Runner->run();
}
?>