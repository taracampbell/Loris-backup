<?php
require_once 'generic_includes.php';
require_once 'CouchDB.class.inc';
require_once 'Database.class.inc';
class CouchDBRadiologicalReviewImporter {
    var $SQLDB; // reference to the database handler, store here instead
                // of using Database::singleton in case it's a mock.
    var $CouchDB; // reference to the CouchDB database handler

    var $Dictionary = array(
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
        
        $radiologicalreview = $this->SQLDB->pselect("SELECT c.PSCID, s.Visit_label,
            eFinal.full_name AS FinalReview_Radiologist, CASE WHEN frr.Review_Done=0 
            THEN 'No' WHEN frr.Review_Done=1 THEN 'Yes' END as FinalReview_Done, 
            frr.Final_Review_Results AS FinalReview_Results, 
            frr.Final_Exclusionary AS FinalReview_ExclusionaryStatus, 
            CASE WHEN frr.SAS=0 THEN 'None' WHEN frr.SAS=1 THEN 'Minimal' 
            WHEN frr.SAS=2 THEN 'Mild' WHEN frr.SAS=3 THEN 'Moderate' 
            WHEN frr.SAS=4 THEN 'Marker' END as FinalReview_SAS, 
            CASE WHEN frr.PVS=0 THEN 'None' WHEN frr.PVS=1 THEN 'Minimal' 
            WHEN frr.PVS=2 THEN 'Mild' WHEN frr.PVS=3 THEN 'Moderate' 
            WHEN frr.PVS=4 THEN 'Marker' END as FinalReview_PVS, 
            frr.Final_Incidental_Findings AS FinalReview_Comment, 
            eExtra.full_name AS ExtraReview_Radiologist, CASE WHEN frr.Review_Done2=0 THEN 'No' 
            WHEN frr.Review_Done2=1 THEN 'Yes' END as ExtraReview_Done, 
            frr.Final_Review_Results2 AS ExtraReview_Results, CASE WHEN frr.SAS2=0 THEN 'None' 
            WHEN frr.SAS2=1 THEN 'Minimal' WHEN frr.SAS2=2 THEN 'Mild' 
            WHEN frr.SAS2=3 THEN 'Moderate' WHEN frr.SAS2=4 THEN 'Marker' END as ExtraReview_SAS, 
            CASE WHEN frr.PVS2=0 THEN 'None' WHEN frr.PVS2=1 THEN 'Minimal' 
            WHEN frr.PVS2=2 THEN 'Mild' WHEN frr.PVS2=3 THEN 'Moderate' 
            WHEN frr.PVS2=4 THEN 'Marker' END as ExtraReview_PVS, 
            frr.Final_Incidental_Findings2 AS ExtraReview_Comment, 
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
            $identifier = array($review['PSCID'], $review['Visit_label']);
            $id = 'Radiological_Review_' . join($identifier, '_');
            unset($review['PSCID']);
            unset($review['Visit_label']);
            $success = $this->CouchDB->replaceDoc($id, array('Meta' => array(
                'DocType' => 'RadiologicalReview',
                'identifier' => $identifier
            ),
                'data' => $review
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