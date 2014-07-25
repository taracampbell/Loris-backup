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
<body background="" class="LoginBackground">
	<div class ="logo">

	</div>
	
 	<div class="navbar navbar-default" role="navigation" style="height:90px">
 		<div class="container">
	 		<a class="navbar-brand" href="#" style="align:center;">
		 		<img src="{$study_logo}" border="0" width="64" height="57" />
		 		{$study_title}
	 		</a>
	 	</div>
 	</div>
 	
 	<div class="container">
 		<div class="row">
		 	<div class="panel panel-default col-md-6 col-md-offset-3">
				<div class="panel-body">
					<div class="col-md-10 col-md-offset-1">
						<img src="images/LORIS_v2.grey.clear.png" class="img-responsive" alt="Responsive image">
					</div>
					</br></br></br>
					<font color="red" align="middle">
						{$error_message}
					</font>
					<form action="{$action}" method="post">
						<div class="form-group">
							<input name="username" class="form-control" type="text" value="{$username}" placeholder="User"/>
						</div>
						<div class="form-group">
							<input name="password" class="form-control" type="password" placeholder="Password"/>
						</div>
						<div class="col-md-4 col-md-offset-4">
							<input class="btn btn-primary btn-block" name="login" type="submit" value="login" />
						</div>
					</form>
					</br></br></br>
					<a href="lost_password.php"><center>Forgot your password?</center></a>
					<hr>
					<div class="login-panel-footer">
						| 
						{foreach from=$studylinks item=link}
							<a href="{$link.url}" target="{$link.windowName}">{$link.label}</a> | 
						{/foreach}
						<p>
							A WebGL-compatible browser is required for full functionality (Mozilla Firefox, Google Chrome)
							</br>
							Powered by LORIS &copy; 2013. All rights reserved.
							</br>
							Created by <a href="http://cbrain.mcgill.ca" target="_blank"> ACElab</a>
							</br>
							Developed at <a href="http://www.mni.mcgill.ca" target="_blank">Montreal Neurological Institute and Hospital</a>
						</p>
					</div>
				</div>
			</div><!-- ./login -->
		</div>
		<div class="login-page-footer">
		 	<div id="investigators">
			 	| 
				{foreach from=$investigators item=investigator}
					{$investigator} |
				{/foreach}
			</div>
			<hr>
			<div id="institutions">
				| 
				{foreach from=$institutions item=institution}
					{$institution} |
				{/foreach}
			</div>
		</div>
 	</div>

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
