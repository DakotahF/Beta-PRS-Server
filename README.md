# PRS Server Beta

*In development

This program is based off of existing code from Daniel Taliun using stat-gen's savvy module(original R package located at : https://github.com/statgen/Rprs)

The PRS pipeline takes in a list of weights to merge as well as a list of dosages to use for polygenic risk score (PRS) calculation. An optional flag for rough ancestry estimation is available to identify individuals whose ancestry is not well reflected in the reference population. 

There is also an option to input a list of weight files and merge them into the format accepted by the PRS pipeline. More information on this feature can be found below. 

## Running the Program

Without merging weights (pre-merged or singular weight file) : 

    ./prs-runner --verbose --dosages dosages_list.txt --output /path/to/desired_output_dir/  --weights trait-weight.txt --ancestry —-ref_dir /path/to/reference-data

With merging weights (requires list of weights to merge in place of weight file) : 

    ./prs-runner --verbose --dosages dosages_list.txt --output /path/to/desired_output_dir/  --weights weight-list.txt --ancestry —-ref_dir /path/to/reference-data --merge

Flags : 

    --weights, -w : required argument, either pre-merged/singular weight file or list of weights to be merged (format of weight list described below), merging feature must be activated by attaching --merge flag 
    --dosages, -d : required argument, list of dosages to use for PRS calculation
    --output : required argument, output directory where results will be stored 
    --ref_dir : directs program to directory with reference data necessary for ancestry calculation (this will be removed in later iterations)
    --refpanel : optional argument, used for reference panel to determine ancestry (right now only supports 1000 genomes)
    --ancestry : boolean flag dictates whether or not ancestry is computed
    --merge : boolean flag dictates whether or not weight files need to be merged or have already been merged

*note, ref_dir is a temporary flag to avoid hardcoding paths for the moment 

### Weightfile Formatting

#### If not using merge-weights feature : 
CHROM	POS	OA	EA	PVALUE	WEIGHT_X175	WEIGHT_X614.5...

22	29392	G	A 	0.0	0.0	0.0 ...

#### If using merge-weights feature : 

phen    phen_loc        format_type

PGS000018       /path/to/weightfile/1/PGS000018.txt  1

x994.2  /path/to/weightfile/2/PheCode_X994.2_SAIGE_b38_5e-2_clumped_r2_0.2.clumped 0

*In the second situation,  the format type is indicative of whether the weights are hosted on our cluster (0) or being pulled from the PGS catalog (1). The PGS catalog was recently finalized, so we will soon be integrating API calls instead. 

### Dosages list Formatting

dosagelist.txt should have the following formatting : 

/path/to/chr1.vcf.gz 

/path/to/chr2.vcf.gz

...

## Output
 
Output directory will be populated with ancestry folders, ancestry results in the outputdir/results/study-superpops.txt and scores will be found in outputdir/dosage_name.scores.txt. 

## References 

Rprs package (source of the bulk of prs-calculation code) : https://github.com/statgen/Rprs

## Notes 

-Ctest testing suites are unfinished and error catching/input checking needs to be better implemented
