#!/bin/perl
# import package
use MIME::QuotedPrint;
use MIME::Base64;

# ask user for critical variables

 open(mailFile,"<$ARGV[0]") or die "open $ARGV[0] failed";

 @allRaw=<mailFile>;
 $fileContent = "";
 foreach $eachline (@allRaw) {
   if(index($eachline,"attachment")>0){
      break;
   }
   $fileContent = $fileContent.$eachline;
 }

 $isMultiPartMail = 0;

 if(index($fileContent,"Encoding: base64")>0){

   $isMultiPartMail = 1;

   $indexPoint = index($fileContent,"Encoding: base64");
   $other1 = substr($fileContent,$indexPoint+18);
   $other2 = substr($other1,0,index($other1,"\n\n"));

   $decode_msg = decode_base64($other2);

   print $decode_msg;
 

 }


if($isMultiPartMail ==0 ) {
 ###### Simple Mail #############
 my $allMsg = "";
 $linenum = 1;
 $isBody=0;
 foreach $lineContext (readline mailFile){

   #print "$linenum ".length($lineContext).": $lineContext";

   if($isBody==1){
      $allMsg = $allMsg.$lineContext;
   }


   if($lineContext eq  "\n" ){
	$isBody=1;
   }

   $linenum++;
 }
 close mailFile;
 ################################
  $msg2 =  decode_base64($allMsg);
   $isHasHREF2 = 0;
   if(index($msg2,"http")>0)
   {
        $isHasHREF2 = 1;
      print $msg2;
   }
   $msg2 = MIME::QuotedPrint::decode($allMsg);
   $isHasHREF2 = 0;
   if(index($msg2,"http")>0)
   {
        $isHasHREF2 = 1;
      print $msg2;
   }
}

  #$base64_decode = decode_base64($allMsg);
  #print $base64_decode;;
  #$decoded = MIME::QuotedPrint::decode($allMsg);
  #print $decoded;

