<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" style="height:100%">
<head>
<meta charset="utf-8"/>
<!-- shortcut icon that displays on the browser window -->
<link rel="shortcut icon" href="images/mni_icon.ico" type="image/ico" />
<link rel="stylesheet" href="bootstrap-3.1.1/css/bootstrap.css">
<link rel="stylesheet" href="css/login.css">
<!-- page title -->
<title>{$title}</title>

<!-- About this Javascript. As time goes on, one may need to update this file with new browsers and latest versions -->
{literal}
<script type="text/javascript">
<!--
var BrowserDetect = {
init: function () {
	      this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
	      this.version = this.searchVersion(navigator.userAgent)
		      || this.searchVersion(navigator.appVersion)
		      || "an unknown version";
	      this.OS = this.searchString(this.dataOS) || "an unknown OS";
      },
searchString: function (data) {
		      for (var i=0;i<data.length;i++)	{
			      var dataString = data[i].string;
			      var dataProp = data[i].prop;
			      this.versionSearchString = data[i].versionSearch || data[i].identity;
			      if (dataString) {
				      if (dataString.indexOf(data[i].subString) != -1)
					      return data[i].identity;
			      }
			      else if (dataProp)
				      return data[i].identity;
		      }
	      },
searchVersion: function (dataString) {
		       var index = dataString.indexOf(this.versionSearchString);
		       if (index == -1) return;
		       return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
	       },
dataBrowser: [
	     { 	string: navigator.userAgent,
subString: "OmniWeb",
	   versionSearch: "OmniWeb/",
	   identity: "OmniWeb"
	     },
	     {
string: navigator.vendor,
	subString: "Apple",
	identity: "Safari"
	     },
	     {
prop: window.opera,
      identity: "Opera"
	     },
	     {
string: navigator.vendor,
	subString: "iCab",
	identity: "iCab"
	     },
	     {
string: navigator.vendor,
	subString: "KDE",
	identity: "Konqueror"
	     },
	     {
string: navigator.userAgent,
	subString: "Firefox",
	identity: "Firefox"
	     },
	     {
string: navigator.vendor,
	subString: "Camino",
	identity: "Camino"
	     },
	     {		// for newer Netscapes (6+)
string: navigator.userAgent,
	subString: "Netscape",
	identity: "Netscape"
	     },
	     {
string: navigator.userAgent,
	subString: "MSIE",
	identity: "Explorer",
	versionSearch: "MSIE"
	     },
	     {
string: navigator.userAgent,
	subString: "Gecko",
	identity: "Mozilla",
	versionSearch: "rv"
	     },
	     { 		// for older Netscapes (4-)
string: navigator.userAgent,
	subString: "Mozilla",
	identity: "Netscape",
	versionSearch: "Mozilla"
	     }
      ],
	      dataOS : [
	      {
string: navigator.platform,
	subString: "Win",
	identity: "Windows"
	      },
	      {
string: navigator.platform,
	subString: "Mac",
	identity: "Mac"
	      },
	      {
string: navigator.platform,
	subString: "Linux",
	identity: "Linux"
	      }
      ]

};
BrowserDetect.init();

// -->
</script>

{/literal}
<meta name="viewport" content="width=device-width, initial-scale=1" />

</head>
<body>
	<div id="wrap">
	 	<div class="navbar navbar-default" role="navigation">
	 		<div class="container">
		 		<a class="navbar-brand">
			 		LORIS
		 		</a>
		 		<button type="button" class="btn btn-default navbar-btn pull-right" data-toggle="modal" data-target="#loginModal">Login</button>
		 	</div>
	 	</div>

	 	<!-- Modal -->
		<div class="modal fade" id="loginModal" tabindex="-1" role="dialog" aria-labelledby="loginModalLabel" aria-hidden="true">
		 	<div class="modal-dialog modal-loris">
		    	<div class="modal-content">
		     		<div class="modal-header">
		       			<button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
		        		<h4 class="modal-title" id="loginModalLabel">Login</h4>
		      		</div>
			      	<div class="modal-body">
			      		<div class="loris-logo">
							<img src="images/LORIS_v2.grey.clear.png" class="img-responsive loris-logo-img" alt="Loris Logo">
						</div>
						{if $error_message neq ""}
							<div class="alert alert-danger" role="alert">{$error_message}</div>
						{/if}
						<div class="form-box">
				        	<form action="{$action}" method="post">
								<input name="username" type="text" value="{$username}" placeholder="username">
		                    	<input name="password" type="password" placeholder="password">
								<input class="btn btn-primary btn-block" name="login" type="submit" value="login" id="modalLoginButton">
							</form>
						</div>
					</div>
			      	<div class="modal-footer">
			        	<a href="lost_password.php">Forgot your password?</a> | <a href="request_account/process_new_account.php">Request account</a>
			      	</div>
		    	</div>
		  	</div>
		</div>
 	
	 	<div class="container" id="page">
	 		<div class="col-lg-8 col-lg-offset-2">
			 	<div class="row">
			 		<div class="panel panel-default">
	 					<div class="panel-body">
	    					<div class="col-lg-4">
	    						<img src="{$study_logo}" class="img-responsive center-block" alt="Loris Logo" id="study-logo">
	    					</div>
	    					<div class="col-lg-8">
	    						<h2>{$study_title}</h2>
	    						<p class="lead">{$description}</p>
	    					</div>
						</div>
					</div>
			 	</div>
				<div class="row">
					<div class="panel panel-default">
						<div class="panel-heading">Investigators</div>
						<div class="panel-body investigators">
							<div class="row">
								<div class="col-md-4">
									<h5>Principal</h5>
									{foreach from=$principal_investigators item=investigator}
										<a href="{$investigator.url}" target="{$investigator.windowName}">{$investigator.label}</a>
										</br>
									{/foreach}
								</div>
								<div class="col-md-4">
									<h5>Behavioural</h5>
									{foreach from=$behavioural_investigators item=investigator}
										<a href="{$investigator.url}" target="{$investigator.windowName}">{$investigator.label}</a>
										</br>
									{/foreach}
								</div>
								<div class="col-md-4">
									<h5>Imaging</h5>
									{foreach from=$imaging_investigators item=investigator}
										<a href="{$investigator.url}" target="{$investigator.windowName}">{$investigator.label}</a>
										</br>
									{/foreach}
								</div>
							</div>
						</div>
					</div>
				</div>
				<div class="row">
					<div class="col-lg-12" id="institutions">
						<ul class="horizontal-slide">
						{foreach from=$institutions item=institution}
							<li><a href="{$institution.url}"><img src="{$institution.logo}" height="120" alt="{$institution.name}"></a></li>
						{/foreach}
						</ul>
					</div>
				</div>
			</div>
		</div>
	</div>

	<div id="footer" class="footer navbar-bottom">
        <ul id="navlist" style="margin-top: 5px; margin-bottom: 2px;" align="center">
            <li id="active">
                |
            </li>
            {foreach from=$studylinks item=link}
                    <li>  
                        <a href="{$link.url}" target="{$link.windowName}">
                            {$link.label}
                        </a> 
                        |
                    </li>
            {/foreach}
            <li><a href="http://journal.frontiersin.org/Journal/10.3389/fninf.2011.00037/full">How to cite LORIS</a> |
        </ul>
        <div align="center">
            A WebGL-compatible browser is required for full functionality (Mozilla Firefox, Google Chrome)
        </div>
        <div align="center">
            Powered by LORIS &copy; 2013. All rights reserved.
        </div>
        <div align="center">
            Created by <a href="http://mcin-cnim.ca/" style="color: #064785" target="_blank"> ACElab</a>
        </div>
        <div align="center">
            Developed at <a href="http://www.mni.mcgill.ca" style="color: #064785" target="_blank">Montreal Neurological Institute and Hospital</a>
        </div>
    </div>
</div>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
<script src="bootstrap-3.1.1/js/bootstrap.min.js"></script>
{literal}
<script type='text/javascript'>
	$("[data-toggle=popover]").popover({html:true});
</script>
{/literal}
{literal}
<script type='text/javascript'>
<!--
if(BrowserDetect.browser == "Explorer") {
	document.write('<p align="center"><b>The browser you are using (Internet Explorer) is not compatible with this database!</b><br>For full functionality please download the latest version of <a href="http://www.mozilla.com/" target="blank">Firefox.</a></p>');
}
// -->	
</script>
{/literal}

{*
	{literal}
	<script type='text/javascript'>
		<!--
		document.write('<p><b>Browser check:</b> You\'re using ' + BrowserDetect.browser + ' ' + BrowserDetect.version + ' on ' + BrowserDetect.OS + '!</p>');
	// -->	
	</script>
	{/literal}
	*}

</body>
</html>
