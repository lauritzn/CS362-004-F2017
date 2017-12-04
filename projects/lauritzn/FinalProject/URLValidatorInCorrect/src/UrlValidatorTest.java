/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;
import java.util.Random;

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 * Tests by CS 362 Group: Erick Brownfield, Nina Lauritzen, Catherine Kaethler
 * 12/3/2017 
 */
public class UrlValidatorTest extends TestCase {

  private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }
  
   /*
    *	Manual tests:
    *   Checks individual valid and invalid URLs and prints results to screen
    */ 
   
   public void testManualTest()
   {
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
		System.out.println("Valid URLS should cause isValid() to return true:");
	
		//Valid URLs
		System.out.println(urlVal.isValid("http://www.amazon.com"));
		System.out.println(urlVal.isValid("http://google.com"));
		System.out.println(urlVal.isValid("http://google.com:80"));
		System.out.println(urlVal.isValid("http://google.com/test1"));
		System.out.println(urlVal.isValid("http://google.com:80/test1"));
		System.out.println(urlVal.isValid("http://google.com:80/test1?action=view"));
		System.out.println(urlVal.isValid("ftp://go.com"));
		System.out.println(urlVal.isValid("ftp://go.com:65535"));
		System.out.println(urlVal.isValid("ftp://go.com/t123"));
		System.out.println(urlVal.isValid("ftp://go.com:65535/t123"));
		System.out.println(urlVal.isValid("ftp://go.com:65535/t123?action=edit&mode=up"));
		System.out.println("");
						
		System.out.println("Invalid URLs should cause isValid() to return false");
	   
		//Invalid URLs
		System.out.println(urlVal.isValid("http://www.amazon.com/page??ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing"));
		System.out.println(urlVal.isValid("http://256.256.256.256"));
		System.out.println(urlVal.isValid("255.255.255.255"));
		System.out.println(urlVal.isValid("3ht://256.256.256.256"));
		System.out.println(urlVal.isValid("3ht://256.256.256.256:80"));
		System.out.println(urlVal.isValid("3ht://256.256.256.256/test1"));
		System.out.println(urlVal.isValid("3ht://256.256.256.256:80/test1"));
		System.out.println(urlVal.isValid("3ht://google.com:80/test1?action=view"));
		System.out.println(urlVal.isValid("http://256.256.256.256:80/test1?action=view"));
		System.out.println(urlVal.isValid("http://google.com:65a/test1?action=view"));
		System.out.println(urlVal.isValid("http://google.com:80/..?action=view"));
		System.out.println(urlVal.isValid("1.2.3.4.5"));
		System.out.println(urlVal.isValid("http:/1.2.3.4.5"));
		System.out.println(urlVal.isValid("http:/1.2.3.4.5:-1"));
		System.out.println(urlVal.isValid("http:/1.2.3.4.5/test1//file"));
		System.out.println(urlVal.isValid("http:/1.2.3.4.5:-1/test1//file"));
		System.out.println(urlVal.isValid("http:/go.com:65535/t123?action=edit&mode=up"));
		System.out.println(urlVal.isValid("ftp://1.2.3.4.5:65535/t123?action=edit&mode=up"));
		System.out.println(urlVal.isValid("ftp://go.com:-1/t123//file?action=edit&mode=up"));
		System.out.println(urlVal.isValid("ftp://go.com:65535/test1//file?action=edit&mode=up"));
	}
   
   /*
    *  Tests input partition #1: scheme (protocols)
    *  Tests both valid and invalid schemes
    *  Tests default schemes and ALLOW_ALL_SCHEMES options 
    */
   
   public void testYourFirstPartition()
   {
		UrlValidator urlValDefault = new UrlValidator();
		UrlValidator urlValAllSchemes = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		int numValid = 3;
		String[] validProtocols = new String[numValid];
		validProtocols[0] = "http://";
		validProtocols[1] = "https://";
		validProtocols[2] ="ftp://";
	  
		int numNonexist = 3;
		String[] nonexistentProtocols = new String[numNonexist];
		nonexistentProtocols[0] = "xxxp://";
		nonexistentProtocols[1] = "htps://";
		nonexistentProtocols[2] = "fftp://";
	  
		int numInvalid = 5;
		String[] invalidProtocols = new String[numInvalid];
		// Contains characters that cannot be part of a protocol, $
		invalidProtocols[0] = "x:$ed://";
		// Contains too many slashes
		invalidProtocols[1] = "http:///";
		// Contains too few slashes
		invalidProtocols[2] = "http:/";
		// Contains internal slashes
		invalidProtocols[3]= "h/t/t/p://";
		// No colon
		invalidProtocols[4] = "http//";

		//Create a base that will be valid
		String base = "en.wikipedia.org/wiki/Software_testing";
		String testString;
	  
		//Loop to check valid schemes
		for (int i = 0; i<numValid; i++)
		{
			testString = validProtocols[i]+base;
		    assertTrue(testString + " valid protocol for default schemes", urlValDefault.isValid(testString));	
			assertTrue(testString + " valid protocol for all schemes", urlValAllSchemes.isValid(testString));
	   }
	  
		//Loop to check nonexistent schemes
		for (int i = 0; i<numNonexist; i++)
		{
			testString = nonexistentProtocols[i]+base;
		    assertFalse(testString + " invalid protocol for default schemes", urlValDefault.isValid(testString));	
			assertTrue(testString + " nonexistent but valid protocol for all schemes", urlValAllSchemes.isValid(testString));	   
		}
	   
	   //Loop to check invalid schemes
	   for (int i = 0; i<numInvalid; i++)
	   {
		  testString = invalidProtocols[i]+base;
		  assertFalse(testString + " invalid protocol for default schemes", urlValDefault.isValid(testString));	
		  assertFalse(testString + " invalid protocol for all schemes", urlValAllSchemes.isValid(testString));			  	   
	   }
	   
   }
   
   /*
    *  Tests input partition #2: authorities
    *  Tests valid and invalid authorities
    *  Tests default and ALLOW_LOCAL_URLS options
    */
   
	public void testYourSecondPartition(){
	   
		UrlValidator urlValDefault = new UrlValidator();
		UrlValidator urlValAllowLocal = new UrlValidator(null, null, UrlValidator.ALLOW_LOCAL_URLS);
		
		//Create an array of valid authorities
		int numValid = 5;
		String[] validAuth = new String[numValid];
		// hostname
		validAuth[0] = "www.amazon.com/";
		// hostname with port
		validAuth[1] = "www.amazon.com:80/";
		validAuth[2] = "www.amazon.com:65535/";
		//IPv4 address: source of example is Wikipedia, https://en.wikipedia.org/wiki/IPv4
		validAuth[3] = "172.16.254.1/";
		// IPv4 address with port
		validAuth[4] = "172.16.254.1:80/";
	   	 
		// Create an array of local authorities	
		int numLocal = 4;
		String[] localAuth = new String[numLocal];
		// localhost with and without port
		localAuth[0] = "localhost/";
		localAuth[1] = "localhost:80/";
		// machine with and without port
		localAuth[2] = "machine/";
		localAuth[3] = "machine:80/";
		
		int numInvalid = 7;
		String[] invalidAuth = new String[numInvalid];
		//Contains characters that cannot be part of an authority: symbols other than the minus sign, punctuation, whitespace
		invalidAuth[0] = "www.ama#on.com/";
		invalidAuth[1] = "www amazon.com/";
		invalidAuth[2] = "www.amazon!.com/";
		// Contains too many slashes
		invalidAuth[3] = "www.google.com//";
		// Contains too few slashes
		invalidAuth[4] = "www.oregonstate.edu";
		// No dot
		invalidAuth[5]= "wwworegonstateedu/";
		// IP address invalid
		invalidAuth[6] = "256.256.256.256/";
	  
		//Create a base that will be valid
		String base1 = "http://";
		String base2 = "wiki/Software_testing";
		String testString;
	   
	   
	   //Loop to check valid authorities
	   for (int i = 0; i<numValid; i++)
	   {
		  testString = base1+validAuth[i];
		  testString += base2;
		  assertTrue(testString + " is valid authority for default", urlValDefault.isValid(testString));	
		  assertTrue(testString + " is valid authority when local is allowed", urlValAllowLocal.isValid(testString));
	   }
	
	 //Loop to check local authorities
	   for (int i = 0; i<numLocal; i++)
	   {
		  testString = base1+localAuth[i];
		  testString += base2;
		  assertFalse(testString + " local is invalid by default", urlValDefault.isValid(testString));	
		  assertTrue(testString + " local is valid when allowed", urlValAllowLocal.isValid(testString));	   
	   }
	   
	   //Loop to check invalid authorities
	   for (int i = 0; i<numInvalid; i++)
	   {
		  testString = base1+invalidAuth[i];
		  testString += base2;
		  assertFalse(testString + " is invalid authority by default", urlValDefault.isValid(testString));	
	   } 
	}

  
	/*
	 * Tests input partition #3: path
	 * Tests valid and invalid paths
	 * Tests default and ALLOW_2_SLASHES options
	 */ 
	
	public void testYourThirdPartition(){
	  
	   // Need to specify no fragments so that path is not interpreted as a fragment
	   UrlValidator urlValDefault = new UrlValidator();
	   UrlValidator urlValTwoSlashes = new UrlValidator(null, null, UrlValidator.ALLOW_2_SLASHES);
	   //Create an array of strings
	   
	   // Valid Paths with one slash
	   int numValid = 5;
	   String[] validPath = new String[numValid];
	   validPath[0] = "wiki";
	   validPath[1] = "wiki.html";
	   validPath[2] = "wiki/Software_Testing";
	   validPath[3] = "this/is/example";
	  validPath[4] = "this/is/the/example.html";
	  
	  // Paths with two slashes
	  int numTwoSlash = 4;
	  String[] twoSlashPath = new String[numTwoSlash];
	 twoSlashPath[0] = "this//is/";
	  twoSlashPath[1] = "this/is//example/";
	  twoSlashPath[2] = "this//is//the//example/";
	  twoSlashPath[3] = "this/is/the//example.html/";
	  
	  int numInvalid = 2;
	  String[] invalidPath = new String[numInvalid];
	  
	  invalidPath[0] = "wiki/SoftwareTest^ing/example/";
	  invalidPath[1] = "wiki/Sof[twareTesting/example/";
	  
	  //Create a base that will be valid
	   String base1 = "http://www.amazon.com/";
	   String testString;
	   
	   //Loop to check valid paths   
	   for (int i = 0; i<numValid; i++)
	   {
		  testString = base1+validPath[i];
		  assertTrue(testString + " is valid path for default", urlValDefault.isValid(testString));	
		  assertTrue(testString + " is valid path when two slashes allowed", urlValTwoSlashes.isValid(testString));
	   }
	
	 //Loop to check paths with two slashes
	   for (int i = 0; i<numTwoSlash; i++)
	   {
		  testString = base1+twoSlashPath[i];
		  assertFalse(testString + " with two slashes is invalid path by default", urlValDefault.isValid(testString));	
		  assertTrue(testString + " is valid path when two slashes allowed", urlValTwoSlashes.isValid(testString));	   
	   }

	 
	   //Loop to check invalid paths
	   for (int i = 0; i<numInvalid; i++)
	   {
		  testString = base1+invalidPath[i];
		  assertFalse(testString + " is invalid path by default", urlValDefault.isValid(testString));				  	   
	   } 
	  
	   
   }      
    
	/*
	 * Tests input partition #4: query string
	 * Tests valid and invalid query strings with default UrlValidator 
	 */
	
	public void testYourFourthPartition(){
	   
	   UrlValidator urlValDefault = new UrlValidator();
	   
	   //Create an array of strings
	   int numValid = 5;
	   String[] validQuery = new String[numValid];
	   
	   // Examples from Wikipedia: https://en.wikipedia.org/wiki/Query_string
	   validQuery[0] = "there?name=ferret";
	   validQuery[1] = "page?name=ferret&color=purple";
	
	   // Other examples
	   validQuery[2] = "page?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	   validQuery[3] = "ref=sr_1_1?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	   validQuery[4] = "page?ie=UTF8;qid=151184224;&sr=8-1;keywords=software+testing";
  
		int numInvalid = 2;
		String[] invalidQuery = new String[numInvalid];
	  
		invalidQuery[0] = "there?^name=ferret&color=purple";
		invalidQuery[1] = "page??ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	  
		//Create a base that will be valid
		String base = "http://en.wikipedia.org/wiki/Software_testing/";
		String testString;
	   
		//Loop to check valid queries
		for (int i = 0; i<numValid; i++)
		{
			testString = base+validQuery[i];
			assertTrue(testString + " is valid query", urlValDefault.isValid(testString));	
		}
	   
		//Loop to check invalid queries
		for (int i = 0; i<numInvalid; i++)
		{
		  testString = base+invalidQuery[i];
		  assertFalse(testString + " is an invalid query", urlValDefault.isValid(testString));	 		  	   
		}
	    
   }
  
	/*
	 * Tests input partition #5: fragment
	 * Tests valid and invalid fragments
	 * Tests with and without NO_FRAGMENTS option set 
	 */
	
	public void testYourFifthPartition(){
	   
	   UrlValidator urlValDefault = new UrlValidator();
	   UrlValidator urlValNoFrag = new UrlValidator(null, null, UrlValidator.NO_FRAGMENTS);
	   
	   //Create an array of strings
	   int numValid = 5;
	   String[] validFragment = new String[numValid];
	   // Examples from Wikipedia https://en.wikipedia.org/wiki/Fragment_identifier
	   validFragment[0] = "#bar";
	   validFragment[1] = "#row=4";
	   validFragment[2] = "#row=5-7";
	   validFragment[3] = "#row=5-*";
	   validFragment[4] = "#t=10,20";
	   
		//Create a base that will be valid
	  	String base = "http://en.wikipedia.org/wiki/Software_testing/";
	  	String testString;
	   
	  	//Source for invalid fragments https://stackoverflow.com/questions/26088849/url-fragment-allowed-characters
	   int numInvalid = 5;
	   String[] invalidFragment = new String[numInvalid];
	   invalidFragment[0] = "#^fragment";
	   invalidFragment[1] = "#fragment<";
	   invalidFragment[2] = "#frag>ment";
	   invalidFragment[3] = "#[fragment";
	   invalidFragment[4] = "#fragment]";
	   
	   //Loop to check valid fragments
	   for (int i = 0; i<numValid; i++)
	   {
		  testString = base+validFragment[i];
		  assertTrue(testString + " is valid URL when fragments allowed", urlValDefault.isValid(testString));
		  assertFalse(testString + " is invalid URL when fragments not allowed", urlValNoFrag.isValid(testString));
	   }
	   
	   // Loop to check invalid fragments
	   for (int i = 0; i<numInvalid; i++)
	   {
		  testString = base+invalidFragment[i];
		  assertFalse(testString + " is invalid URL when fragments allowed", urlValDefault.isValid(testString));
		  assertFalse(testString + " is invalid URL when fragments not allowed", urlValNoFrag.isValid(testString));
	   }
	}
 
	/*
	 * Tests a series of invalid URLS each of which has one invalid component
	 * Invalid URLs are built by combining random valid components with each
	 * invalid component in turn
	 * Uses a custom set of protocols
	 */
	
	public void testIsValid1()
	{
	   	
		// valid components


		// protocol
		//Source: https://www.iana.org/assignments/uri-schemes/uri-schemes.xhtml

		String[] protocol = { 
		"aaa",
		"aaas",	
		"about",	
		"acap",
		"acct",
		"acr",
		"adiumxtra",
		"afp",
		"afs",
		"aim",
		"appdata",
		"apt",
		"attachment",
		"aw",
		"barion",
		"beshare",
		"bitcoin",
		"blob",
		"bolo",
		"browserext",
		"callto",
		"cap",
		"chrome",
		"chrome-extension",
		"cid",
		"coap",
		"coap+tcp",
		"coaps",
		"coaps+tcp",
		"com-eventbrite-attendee",
		"content",
		"conti",
		"crid",
		"cvs",
		"data",
		"dav",
		"diaspora",
		"dict",
		"dis",
		"dlna-playcontainer",
		"dlna-playsingle",
		"dns",
		"dntp",
		"dtn",
		"dvb",
		"ed2k",
		"example",
		"facetime",
		"fax",
		"feed",
		"feedready",
		"file",
		"filesystem",
		"finger",
		"fish",
		"ftp",
		"geo",
		"gg",
		"git",
		"gizmoproject",
		"go",
		"gopher",
		"graph",
		"gtalk",
		"h323",
		"ham",
		"hcp",
		"http",
		"https",
		"hxxp",
		"hxxps",
		"hydrazone",
		"iax",
		"icap",
		"icon",
		"im",
		"imap",
		"info",
		"iotdisco",
		"ipn",
		"ipp",
		"ipps",
		"irc",
		"irc6",
		"ircs",
		"iris",
		"iris.beep",
		"iris.lwz",
		"iris.xpc",
		"iris.xpcs",
		"isostore",
		"itms",
		"jabber",
		"jar",
		"jms",
		"keyparc",
		"lastfm",
		"ldap",
		"ldaps",
		"lvlt",
		"magnet",
		"mailserver",
		"mailto",
		"maps",
		"market",
		"message",
		"mid",
		"mms",
		"modem",
		"mongodb",
		"moz",
		"ms-access",
		"ms-browser-extension",
		"ms-drive-to",
		"ms-enrollment",
		"ms-excel",
		"ms-gamebarservices",
		"ms-gamingoverlay",
		"ms-getoffice",
		"ms-help",
		"ms-infopath",
		"ms-inputapp",
		"ms-media-stream-id",
		"ms-officeapp",
		"ms-people",
		"ms-project",
		"ms-powerpoint",
		"ms-publisher",
		"ms-search-repair",
		"ms-secondary-screen-controller",
		"ms-secondary-screen-setup",
		"ms-settings",
		"ms-settings-airplanemode",
		"ms-settings-bluetooth",
		"ms-settings-camera",
		"ms-settings-cellular",
		"ms-settings-cloudstorage",
		"ms-settings-connectabledevices",
		"ms-settings-displays-topology",
		"ms-settings-emailandaccounts",
		"ms-settings-language",
		"ms-settings-location",
		"ms-settings-lock",
		"ms-settings-nfctransactions",
		"ms-settings-notifications",
		"ms-settings-power",
		"ms-settings-privacy",
		"ms-settings-proximity",
		"ms-settings-screenrotation",
		"ms-settings-wifi",
		"ms-settings-workplace",
		"ms-spd",
		"ms-sttoverlay",
		"ms-transit-to",
		"ms-virtualtouchpad",
		"ms-visio",
		"ms-walk-to",
		"ms-whiteboard",
		"ms-whiteboard-cmd",
		"ms-word",
		"msnim",
		"msrp",
		"msrps",
		"mtqp",
		"mumble",
		"mupdate",
		"mvn",
		"news",
		"nfs",
		"ni",
		"nih",
		"nntp",
		"notes",
		"ocf",
		"oid",
		"onenote",
		"onenote-cmd",
		"opaquelocktoken",
		"pack",
		"palm",
		"paparazzi",
		"pkcs11",
		"platform",
		"pop",
		"pres",
		"prospero",
		"proxy",
		"pwid",
		"psyc",
		"qb",
		"query",
		"redis",
		"rediss",
		"reload",
		"res",
		"resource",
		"rmi",
		"rsync",
		"rtmfp",
		"rtmp",
		"rtsp",
		"rtsps",
		"rtspu",
		"secondlife",
		"service",
		"session",
		"sftp",
		"sgn",
		"shttp",
		"sieve",
		"sip",
		"sips",
		"skype",
		"smb",
		"sms",
		"smtp",
		"snews",
		"snmp",
		"soap.beep",
		"soap.beeps",
		"soldat",
		"spotify",
		"ssh",
		"steam",
		"stun",
		"stuns",
		"submit",
		"svn",
		"tag",
		"teamspeak",
		"tel",
		"teliaeid",
		"telnet",
		"tftp",
		"things",
		"thismessage",
		"tip",
		"tn3270",
		"tool",
		"turn",
		"turns",
		"tv",
		"udp",
		"unreal",
		"urn",
		"ut2004",
		"v-event",
		"vemmi",
		"ventrilo",
		"videotex",
		"vnc",
		"view-source",
		"wais",
		"webcal",
		"wpid",
		"ws",
		"wss",
		"wtai",
		"wyciwyg",
		"xcon",
		"xcon-userid",
		"xfire",
		"xmlrpc.beep",
		"xmlrpc.beeps",
		"xmpp",
		"xri",
		"ymsgr",
		"z39.50",
		"z39.50r",
		"z39.50s",
	};


	// authority - domain + tld
	// String[] domain = new String[10];
	// domain[0]="amazon";
	// domain[1]="google";
	// domain[2]="yahoo";
	//Source for TLDs: https://en.wikipedia.org/wiki/Country_code_top-level_domain


	String[] tld = new String[244];
	tld[0]=".ac";
	tld[1]=".ad";
	tld[2]=".ae";
	tld[3]=".af";
	tld[4]=".ag";
	tld[5]=".ai";
	tld[6]=".al";
	tld[7]=".am";
	tld[8]=".ao";
	tld[9]=".aq";
	tld[10]=".ar";
	tld[11]=".as";
	tld[12]=".at";
	tld[13]=".au";
	tld[14]=".aw";
	tld[15]=".ax";
	tld[16]=".az";
	tld[17]=".ba";
	tld[18]=".bb";
	tld[19]=".bd";
	tld[20]=".be";
	tld[21]=".bf";
	tld[22]=".bg";
	tld[23]=".bh";
	tld[24]=".bi";
	tld[25]=".bj";
	tld[26]=".bm";
	tld[27]=".bn";
	tld[28]=".bo";
	tld[29]=".br";
	tld[30]=".bs";
	tld[31]=".bt";
	tld[32]=".bw";
	tld[33]=".by";
	tld[34]=".bz";
	tld[35]=".ca";
	tld[36]=".cc";
	tld[37]=".cd";
	tld[38]=".cf";
	tld[39]=".cg";
	tld[40]=".ch";
	tld[41]=".ci";
	tld[42]=".ck";
	tld[43]=".cl";
	tld[44]=".cm";
	tld[45]=".cn";
	tld[46]=".co";
	tld[47]=".cr";
	tld[48]=".cu";
	tld[49]=".cv";
	tld[50]=".cw";
	tld[51]=".cx";
	tld[52]=".cy";
	tld[53]=".cz";
	tld[54]=".de";
	tld[55]=".dj";
	tld[56]=".dk";
	tld[57]=".dm";
	tld[58]=".do";
	tld[59]=".dz";
	tld[60]=".ec";
	tld[61]=".ee";
	tld[62]=".eg";
	tld[63]=".er";
	tld[64]=".es";
	tld[65]=".et";
	tld[66]=".eu";
	tld[67]=".fi";
	tld[68]=".fj";
	tld[69]=".fk";
	tld[70]=".fm";
	tld[71]=".fo";
	tld[72]=".fr";
	tld[73]=".ga";
	tld[74]=".gd";
	tld[75]=".ge";
	tld[76]=".gf";
	tld[77]=".gg";
	tld[78]=".gh";
	tld[79]=".gi";
	tld[80]=".gl";
	tld[81]=".gm";
	tld[82]=".gn";
	tld[83]=".gp";
	tld[84]=".gq";
	tld[85]=".gr";
	tld[86]=".gs";
	tld[87]=".gt";
	tld[88]=".gu";
	tld[89]=".gw";
	tld[90]=".gy";
	tld[91]=".hk";
	tld[92]=".hm";
	tld[93]=".hn";
	tld[94]=".hr";
	tld[95]=".ht";
	tld[96]=".hu";
	tld[97]=".id";
	tld[98]=".ie";
	tld[99]=".il";
	tld[100]=".im";
	tld[101]=".in";
	tld[102]=".io";
	tld[103]=".iq";
	tld[104]=".ir";
	tld[105]=".is";
	tld[106]=".it";
	tld[107]=".je";
	tld[108]=".jm";
	tld[109]=".jo";
	tld[110]=".jp";
	tld[111]=".ke";
	tld[112]=".kg";
	tld[113]=".kh";
	tld[114]=".ki";
	tld[115]=".km";
	tld[116]=".kn";
	tld[117]=".kp";
	tld[118]=".kr";
	tld[119]=".kw";
	tld[120]=".ky";
	tld[121]=".kz";
	tld[122]=".la";
	tld[123]=".lb";
	tld[124]=".lc";
	tld[125]=".li";
	tld[126]=".lk";
	tld[127]=".lr";
	tld[128]=".ls";
	tld[129]=".lt";
	tld[130]=".lu";
	tld[131]=".lv";
	tld[132]=".ly";
	tld[133]=".ma";
	tld[134]=".mc";
	tld[135]=".md";
	tld[136]=".me";
	tld[137]=".mg";
	tld[138]=".mh";
	tld[139]=".mk";
	tld[140]=".ml";
	tld[141]=".mm";
	tld[142]=".mn";
	tld[143]=".mo";
	tld[144]=".mp";
	tld[145]=".mq";
	tld[146]=".mr";
	tld[147]=".ms";
	tld[148]=".mt";
	tld[149]=".mu";
	tld[150]=".mv";
	tld[151]=".mw";
	tld[152]=".mx";
	tld[153]=".my";
	tld[154]=".mz";
	tld[155]=".na";
	tld[156]=".nc";
	tld[157]=".ne";
	tld[158]=".nf";
	tld[159]=".ng";
	tld[160]=".ni";
	tld[161]=".nl";
	tld[162]=".no";
	tld[163]=".np";
	tld[164]=".nr";
	tld[165]=".nu";
	tld[166]=".nz";
	tld[167]=".om";
	tld[168]=".pa";
	tld[169]=".pe";
	tld[170]=".pf";
	tld[171]=".pg ";
	tld[172]=".ph";
	tld[173]=".pk";
	tld[174]=".pl";
	tld[175]=".pm";
	tld[176]=".pn";
	tld[177]=".pr";
	tld[178]=".ps";
	tld[179]=".pt";
	tld[180]=".pw";
	tld[181]=".py";
	tld[182]=".qa";
	tld[183]=".re";
	tld[184]=".ro";
	tld[185]=".rs";
	tld[186]=".ru";
	tld[187]=".rw";
	tld[188]=".sa";
	tld[189]=".sb";
	tld[190]=".sc";
	tld[191]=".sd";
	tld[192]=".se";
	tld[193]=".sg";
	tld[194]=".sh";
	tld[195]=".si";
	tld[196]=".sk";
	tld[197]=".sl";
	tld[198]=".sm";
	tld[199]=".sn";
	tld[200]=".so";
	tld[201]=".sr"; 
	tld[202]=".ss";
	tld[203]=".st";
	tld[204]=".sv";
	tld[205]=".sx";
	tld[206]=".sy";
	tld[207]=".sz";
	tld[208]=".tc";
	tld[209]=".td";
	tld[210]=".tf";
	tld[211]=".tg";
	tld[212]=".th";
	tld[213]=".tj";
	tld[214]=".tk";
	tld[215]=".tl";
	tld[216]=".tm";
	tld[217]=".tn";
	tld[218]=".to";
	tld[219]=".tr";
	tld[220]=".tt";
	tld[221]=".tv";
	tld[222]=".tw";
	tld[223]=".tz";
	tld[224]=".ua";
	tld[225]=".ug";
	tld[226]=".uk";
	tld[227]=".us";
	tld[228]=".uy";
	tld[229]=".uz";
	tld[230]=".va";
	tld[231]=".vc";
	tld[232]=".ve";
	tld[233]=".vg";
	tld[234]=".vi";
	tld[235]=".vn";
	tld[236]=".vu";
	tld[237]=".wf";
	tld[238]=".ws";
	tld[239]=".ye";
	tld[240]=".yt";
	tld[241]=".za";
	tld[242]=".zm";
	tld[243]=".zw";
	
	// port 
	String[] port = new String[7];

	port[0] = ":0";
	port[1] = ":1";
	port[2] = ":12";
	port[3] = ":123";
	port[4] = ":1234";
	port[5] = ":12345";
	port[6] = ":65535";
		
	// path 
	String[] path = new String[3];

	path[0] = "/test1";
	path[1] = "/test1/test2";
	path[2] = "/test1/test/2/test3";
	
	// query
	String[] query = new String[5];

	query[0] = "there?name=ferret";
	query[1] = "page?name=ferret&color=purple";
	query[2] = "page?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	query[3] = "ref=sr_1_1?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	query[4] = "page?ie=UTF8;qid=151184224;&sr=8-1;keywords=software+testing";
	
	// invalid components

	// invalid protocol
	
	String[] invalidProtocol = new String[8];

	invalidProtocol[0] = "xxxp://";
	invalidProtocol[1] = "htps://";
	invalidProtocol[2] = "fftp://";
	invalidProtocol[3] = "x:$ed://";
	invalidProtocol[4] = "http:///";
	invalidProtocol[5] = "http:/";
	invalidProtocol[6]= "h/t/t/p://";
	invalidProtocol[7] = "http//";
	
	// invalid authority
	String[] invalidAuth = new String[5];

	invalidAuth[0] = "www.ama#on.com";
	invalidAuth[1] = "www amazon.com";
	invalidAuth[2] = "www.amazon!.com";
	invalidAuth[3] = "www.google.com/";
	invalidAuth[4]= "wwworegonstateedu";

	// invalid port 
	String[] invalidPort = new String[5];

	invalidPort[0] = ":65a";
	invalidPort[1] = ":65536";
	invalidPort[2] = ":-1";
	invalidPort[3] = ":99999";
	invalidPort[4] = "65"; // no ":" in front

	// invalid path
	String[] invalidPath = new String[5];

	invalidPath[0] = "/..";
	invalidPath[1] = "/../";
	invalidPath[2] = "/../file";
	invalidPath[3] = "/#";
	invalidPath[4] = "/#/file";

	// invalid query
	String[] invalidQuery = new String[2];
	invalidQuery[0] = "page??ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	invalidQuery[1] = "page?ie=UTF8&&qid=1511842248&sr=8-1&keywords=software+testing";

	// valid component array
	String[][] validParts = {protocol, tld, port, path, query};

	// invalid component array
	String[][] invalidParts = {invalidProtocol, invalidAuth, invalidPort, invalidPath, invalidQuery};

	// increment invalid components and combine with random valid components

	// seeding random generator
	Random rand = new Random();

	// loop through all invalid components
	for (int i = 0; i < invalidParts.length; i++) 
	{
		
		// loop through an invalid component
		for (int j = 0; j < (invalidParts[i].length); j++) 
		{
			StringBuilder urlBuilder = new StringBuilder();
			// loop through all valid components to build url
			for (int k = 0; k < validParts.length; k++) 
			{
				// if i = k, use invalid component
				if (i == k) 
				{
					urlBuilder.append(invalidParts[i][j]);
					// else, use valid component
				}
				else 
				{
					// generate random number for valid component
					int index = rand.nextInt(validParts[k].length);
					// if appending tld, need to append domain name first
					if (k == 1) 
					{
						urlBuilder.append("google");
					}
					// append valid component
					//System.out.println(validParts[k][index]);
					urlBuilder.append(validParts[k][index]);
					if (k == 0) {
						urlBuilder.append("://");
					}
						
				}
			}
			// convert to string for testing
			String testUrl = urlBuilder.toString();
		
			// test the url
			UrlValidator urlValDefault = new UrlValidator(protocol);
			assertFalse("URL should be found invalid: " + testUrl, urlValDefault.isValid(testUrl));
		}
	  }
	}

	/*
	 * Tests a series of valid URLS composed of only valid components
	 * Valid URLs are built by combining random valid components with each
	 * valid component in turn
	 * Uses a custom set of protocols and tests all country TLDs
	 */
	
	public void testIsValid2()
	{
	   	
		// valid components

		// protocol
		//Source: https://www.iana.org/assignments/uri-schemes/uri-schemes.xhtml
		String[] protocol = { 
		"aaa",
		"aaas",	
		"about",	
		"acap",
		"acct",
		"acr",
		"adiumxtra",
		"afp",
		"afs",
		"aim",
		"appdata",
		"apt",
		"attachment",
		"aw",
		"barion",
		"beshare",
		"bitcoin",
		"blob",
		"bolo",
		"browserext",
		"callto",
		"cap",
		"chrome",
		"chrome-extension",
		"cid",
		"coap",
		"coap+tcp",
		"coaps",
		"coaps+tcp",
		"com-eventbrite-attendee",
		"content",
		"conti",
		"crid",
		"cvs",
		"data",
		"dav",
		"diaspora",
		"dict",
		"dis",
		"dlna-playcontainer",
		"dlna-playsingle",
		"dns",
		"dntp",
		"dtn",
		"dvb",
		"ed2k",
		"example",
		"facetime",
		"fax",
		"feed",
		"feedready",
		"file",
		"filesystem",
		"finger",
		"fish",
		"ftp",
		"geo",
		"gg",
		"git",
		"gizmoproject",
		"go",
		"gopher",
		"graph",
		"gtalk",
		"h323",
		"ham",
		"hcp",
		"http",
		"https",
		"hxxp",
		"hxxps",
		"hydrazone",
		"iax",
		"icap",
		"icon",
		"im",
		"imap",
		"info",
		"iotdisco",
		"ipn",
		"ipp",
		"ipps",
		"irc",
		"irc6",
		"ircs",
		"iris",
		"iris.beep",
		"iris.lwz",
		"iris.xpc",
		"iris.xpcs",
		"isostore",
		"itms",
		"jabber",
		"jar",
		"jms",
		"keyparc",
		"lastfm",
		"ldap",
		"ldaps",
		"lvlt",
		"magnet",
		"mailserver",
		"mailto",
		"maps",
		"market",
		"message",
		"mid",
		"mms",
		"modem",
		"mongodb",
		"moz",
		"ms-access",
		"ms-browser-extension",
		"ms-drive-to",
		"ms-enrollment",
		"ms-excel",
		"ms-gamebarservices",
		"ms-gamingoverlay",
		"ms-getoffice",
		"ms-help",
		"ms-infopath",
		"ms-inputapp",
		"ms-media-stream-id",
		"ms-officeapp",
		"ms-people",
		"ms-project",
		"ms-powerpoint",
		"ms-publisher",
		"ms-search-repair",
		"ms-secondary-screen-controller",
		"ms-secondary-screen-setup",
		"ms-settings",
		"ms-settings-airplanemode",
		"ms-settings-bluetooth",
		"ms-settings-camera",
		"ms-settings-cellular",
		"ms-settings-cloudstorage",
		"ms-settings-connectabledevices",
		"ms-settings-displays-topology",
		"ms-settings-emailandaccounts",
		"ms-settings-language",
		"ms-settings-location",
		"ms-settings-lock",
		"ms-settings-nfctransactions",
		"ms-settings-notifications",
		"ms-settings-power",
		"ms-settings-privacy",
		"ms-settings-proximity",
		"ms-settings-screenrotation",
		"ms-settings-wifi",
		"ms-settings-workplace",
		"ms-spd",
		"ms-sttoverlay",
		"ms-transit-to",
		"ms-virtualtouchpad",
		"ms-visio",
		"ms-walk-to",
		"ms-whiteboard",
		"ms-whiteboard-cmd",
		"ms-word",
		"msnim",
		"msrp",
		"msrps",
		"mtqp",
		"mumble",
		"mupdate",
		"mvn",
		"news",
		"nfs",
		"ni",
		"nih",
		"nntp",
		"notes",
		"ocf",
		"oid",
		"onenote",
		"onenote-cmd",
		"opaquelocktoken",
		"pack",
		"palm",
		"paparazzi",
		"pkcs11",
		"platform",
		"pop",
		"pres",
		"prospero",
		"proxy",
		"pwid",
		"psyc",
		"qb",
		"query",
		"redis",
		"rediss",
		"reload",
		"res",
		"resource",
		"rmi",
		"rsync",
		"rtmfp",
		"rtmp",
		"rtsp",
		"rtsps",
		"rtspu",
		"secondlife",
		"service",
		"session",
		"sftp",
		"sgn",
		"shttp",
		"sieve",
		"sip",
		"sips",
		"skype",
		"smb",
		"sms",
		"smtp",
		"snews",
		"snmp",
		"soap.beep",
		"soap.beeps",
		"soldat",
		"spotify",
		"ssh",
		"steam",
		"stun",
		"stuns",
		"submit",
		"svn",
		"tag",
		"teamspeak",
		"tel",
		"teliaeid",
		"telnet",
		"tftp",
		"things",
		"thismessage",
		"tip",
		"tn3270",
		"tool",
		"turn",
		"turns",
		"tv",
		"udp",
		"unreal",
		"urn",
		"ut2004",
		"v-event",
		"vemmi",
		"ventrilo",
		"videotex",
		"vnc",
		"view-source",
		"wais",
		"webcal",
		"wpid",
		"ws",
		"wss",
		"wtai",
		"wyciwyg",
		"xcon",
		"xcon-userid",
		"xfire",
		"xmlrpc.beep",
		"xmlrpc.beeps",
		"xmpp",
		"xri",
		"ymsgr",
		"z39.50",
		"z39.50r",
		"z39.50s",
	};


	// authority - domain + tld
	// String[] domain = new String[10];
	// domain[0]="amazon";
	// domain[1]="google";
	// domain[2]="yahoo";
	//Source for TLDs: https://en.wikipedia.org/wiki/Country_code_top-level_domain


	String[] tld = new String[244];
	tld[0]=".ac";
	tld[1]=".ad";
	tld[2]=".ae";
	tld[3]=".af";
	tld[4]=".ag";
	tld[5]=".ai";
	tld[6]=".al";
	tld[7]=".am";
	tld[8]=".ao";
	tld[9]=".aq";
	tld[10]=".ar";
	tld[11]=".as";
	tld[12]=".at";
	tld[13]=".au";
	tld[14]=".aw";
	tld[15]=".ax";
	tld[16]=".az";
	tld[17]=".ba";
	tld[18]=".bb";
	tld[19]=".bd";
	tld[20]=".be";
	tld[21]=".bf";
	tld[22]=".bg";
	tld[23]=".bh";
	tld[24]=".bi";
	tld[25]=".bj";
	tld[26]=".bm";
	tld[27]=".bn";
	tld[28]=".bo";
	tld[29]=".br";
	tld[30]=".bs";
	tld[31]=".bt";
	tld[32]=".bw";
	tld[33]=".by";
	tld[34]=".bz";
	tld[35]=".ca";
	tld[36]=".cc";
	tld[37]=".cd";
	tld[38]=".cf";
	tld[39]=".cg";
	tld[40]=".ch";
	tld[41]=".ci";
	tld[42]=".ck";
	tld[43]=".cl";
	tld[44]=".cm";
	tld[45]=".cn";
	tld[46]=".co";
	tld[47]=".cr";
	tld[48]=".cu";
	tld[49]=".cv";
	tld[50]=".cw";
	tld[51]=".cx";
	tld[52]=".cy";
	tld[53]=".cz";
	tld[54]=".de";
	tld[55]=".dj";
	tld[56]=".dk";
	tld[57]=".dm";
	tld[58]=".do";
	tld[59]=".dz";
	tld[60]=".ec";
	tld[61]=".ee";
	tld[62]=".eg";
	tld[63]=".er";
	tld[64]=".es";
	tld[65]=".et";
	tld[66]=".eu";
	tld[67]=".fi";
	tld[68]=".fj";
	tld[69]=".fk";
	tld[70]=".fm";
	tld[71]=".fo";
	tld[72]=".fr";
	tld[73]=".ga";
	tld[74]=".gd";
	tld[75]=".ge";
	tld[76]=".gf";
	tld[77]=".gg";
	tld[78]=".gh";
	tld[79]=".gi";
	tld[80]=".gl";
	tld[81]=".gm";
	tld[82]=".gn";
	tld[83]=".gp";
	tld[84]=".gq";
	tld[85]=".gr";
	tld[86]=".gs";
	tld[87]=".gt";
	tld[88]=".gu";
	tld[89]=".gw";
	tld[90]=".gy";
	tld[91]=".hk";
	tld[92]=".hm";
	tld[93]=".hn";
	tld[94]=".hr";
	tld[95]=".ht";
	tld[96]=".hu";
	tld[97]=".id";
	tld[98]=".ie";
	tld[99]=".il";
	tld[100]=".im";
	tld[101]=".in";
	tld[102]=".io";
	tld[103]=".iq";
	tld[104]=".ir";
	tld[105]=".is";
	tld[106]=".it";
	tld[107]=".je";
	tld[108]=".jm";
	tld[109]=".jo";
	tld[110]=".jp";
	tld[111]=".ke";
	tld[112]=".kg";
	tld[113]=".kh";
	tld[114]=".ki";
	tld[115]=".km";
	tld[116]=".kn";
	tld[117]=".kp";
	tld[118]=".kr";
	tld[119]=".kw";
	tld[120]=".ky";
	tld[121]=".kz";
	tld[122]=".la";
	tld[123]=".lb";
	tld[124]=".lc";
	tld[125]=".li";
	tld[126]=".lk";
	tld[127]=".lr";
	tld[128]=".ls";
	tld[129]=".lt";
	tld[130]=".lu";
	tld[131]=".lv";
	tld[132]=".ly";
	tld[133]=".ma";
	tld[134]=".mc";
	tld[135]=".md";
	tld[136]=".me";
	tld[137]=".mg";
	tld[138]=".mh";
	tld[139]=".mk";
	tld[140]=".ml";
	tld[141]=".mm";
	tld[142]=".mn";
	tld[143]=".mo";
	tld[144]=".mp";
	tld[145]=".mq";
	tld[146]=".mr";
	tld[147]=".ms";
	tld[148]=".mt";
	tld[149]=".mu";
	tld[150]=".mv";
	tld[151]=".mw";
	tld[152]=".mx";
	tld[153]=".my";
	tld[154]=".mz";
	tld[155]=".na";
	tld[156]=".nc";
	tld[157]=".ne";
	tld[158]=".nf";
	tld[159]=".ng";
	tld[160]=".ni";
	tld[161]=".nl";
	tld[162]=".no";
	tld[163]=".np";
	tld[164]=".nr";
	tld[165]=".nu";
	tld[166]=".nz";
	tld[167]=".om";
	tld[168]=".pa";
	tld[169]=".pe";
	tld[170]=".pf";
	tld[171]=".pg ";
	tld[172]=".ph";
	tld[173]=".pk";
	tld[174]=".pl";
	tld[175]=".pm";
	tld[176]=".pn";
	tld[177]=".pr";
	tld[178]=".ps";
	tld[179]=".pt";
	tld[180]=".pw";
	tld[181]=".py";
	tld[182]=".qa";
	tld[183]=".re";
	tld[184]=".ro";
	tld[185]=".rs";
	tld[186]=".ru";
	tld[187]=".rw";
	tld[188]=".sa";
	tld[189]=".sb";
	tld[190]=".sc";
	tld[191]=".sd";
	tld[192]=".se";
	tld[193]=".sg";
	tld[194]=".sh";
	tld[195]=".si";
	tld[196]=".sk";
	tld[197]=".sl";
	tld[198]=".sm";
	tld[199]=".sn";
	tld[200]=".so";
	tld[201]=".sr"; 
	tld[202]=".ss";
	tld[203]=".st";
	tld[204]=".sv";
	tld[205]=".sx";
	tld[206]=".sy";
	tld[207]=".sz";
	tld[208]=".tc";
	tld[209]=".td";
	tld[210]=".tf";
	tld[211]=".tg";
	tld[212]=".th";
	tld[213]=".tj";
	tld[214]=".tk";
	tld[215]=".tl";
	tld[216]=".tm";
	tld[217]=".tn";
	tld[218]=".to";
	tld[219]=".tr";
	tld[220]=".tt";
	tld[221]=".tv";
	tld[222]=".tw";
	tld[223]=".tz";
	tld[224]=".ua";
	tld[225]=".ug";
	tld[226]=".uk";
	tld[227]=".us";
	tld[228]=".uy";
	tld[229]=".uz";
	tld[230]=".va";
	tld[231]=".vc";
	tld[232]=".ve";
	tld[233]=".vg";
	tld[234]=".vi";
	tld[235]=".vn";
	tld[236]=".vu";
	tld[237]=".wf";
	tld[238]=".ws";
	tld[239]=".ye";
	tld[240]=".yt";
	tld[241]=".za";
	tld[242]=".zm";
	tld[243]=".zw";
	
	// port 
	String[] port = new String[7];

	port[0] = ":0";
	port[1] = ":1";
	port[2] = ":12";
	port[3] = ":123";
	port[4] = ":1234";
	port[5] = ":12345";
	port[6] = ":65535";
		
	// path 
	String[] path = new String[3];

	path[0] = "/test1";
	path[1] = "/test1/test2";
	path[2] = "/test1/test/2/test3";
	
	// query
	String[] query = new String[5];

	query[0] = "there?name=ferret";
	query[1] = "page?name=ferret&color=purple";
	query[2] = "page?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	query[3] = "ref=sr_1_1?ie=UTF8&qid=1511842248&sr=8-1&keywords=software+testing";
	query[4] = "page?ie=UTF8;qid=151184224;&sr=8-1;keywords=software+testing";
	
	// valid component array
	String[][] validParts = {protocol, tld, port, path, query};

	// increment valid components and combine with random valid components

	// seeding random generator
	Random rand = new Random();

	// loop through all valid components
	for (int i = 0; i < validParts.length; i++) 
	{	
		// loop through a valid component
		for (int j = 0; j < (validParts[i].length); j++) 
		{
			StringBuilder urlBuilder = new StringBuilder();
			// loop through all valid components to build url
			for (int k = 0; k < validParts.length; k++) 
			{
				// if i = k, use specific valid component
				if (i == k) 
				{
					if (k == 1) 
					{
						urlBuilder.append("google");
					}
					urlBuilder.append(validParts[i][j]);
					if(k == 0)
					{
						urlBuilder.append("://");
					}
					
					// else, use random valid component
				}
				else 
				{
					// generate random number for valid component
					int index = rand.nextInt(validParts[k].length);
					// if appending tld, need to append domain name first
					if (k == 1) 
					{
						urlBuilder.append("google");
					}
					// append valid component
					//System.out.println(validParts[k][index]);
					urlBuilder.append(validParts[k][index]);
					if (k == 0) {
						urlBuilder.append("://");
					}
						
				}
						
			}
			// convert to string for testing
			String testUrl = urlBuilder.toString();
		
			// test the url
			UrlValidator urlVal = new UrlValidator(protocol);
			if (!urlVal.isValid(testUrl))
			{	
				assertTrue("URL should pass as valid: " + testUrl, urlVal.isValid(testUrl));
			}	
		}
	}
  }

}


