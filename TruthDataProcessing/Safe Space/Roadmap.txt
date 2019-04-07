Main file must be link to whole process.

Data will be stored in:
    > Test_Name
        > GPSfile.txt
        > truth.json
        > VANTAGEfile.jason

Truthfile Format:
    jsonencode(truth):
        truth: a 1xN struct where each ith element corresponds to the ith
               time and N is the number of CubeSats
            truth(i).t: the ith time
            truth(i).pos: this is a struct holds all N position vectors:
                truth(i).pos.Cubesat_j: the 3x1 position vector [x;y;z] of
                                        the jth CubeSat

Options:
    > Import truth data:
        X Select file
        X Extract relevant data
        X Data is saved to truth.json automagically
    > Look at VANTAGE performance for this test
        - Probably plots of stuff. IDK yet
        - If VANTAGE not available plot truth by itself
    > Correlate performance over several tests
        - Select relevant folders (include "all" button)
        - Do covariance analysis and make error plots