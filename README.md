# PRS Server Beta

*In development

This program is based off of existing code from Daniel Taliun using stat-gen's savvy module(original R package located at : https://github.com/statgen/Rprs)

The PRS pipeline takes in a list of weights to merge as well as a list of dosages on which to calculate PRS. An optional flag for rough ancestry estimation is available to ensure theend-user is aware of which individual's risk scores may be based on a population of which they are not a member. 

##Running the Program

Sample command to run program : 

    ./prs-calc --weights --dosage --out --ancestry --verbose

Parameters : 
    --weights, -w : 
    --dosage, -d :
    --out :  
    --ancestry : 


###Weightfile Formatting

weightlist.txt should have the following formatting : 



###Dosages list Formatting

dosagelist.txt should have the following formatting : 


##Output
 
Output directory will be populated with ancestry folders, 


##References 

##Notes 

