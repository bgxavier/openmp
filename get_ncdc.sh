#/usr/bin/bash
DATASET_FOLDER="/Users/bgxavier/projects/OpenMP_MPI/dataset"

#for i in {1954..1955}
#do
    wget -r -c -N -np -nH --cut-dirs=3 -R index.html ftp://ftp.ncdc.noaa.gov/pub/data/noaa/$1/
    mkdir -p $DATASET_FOLDER/files/$1
    cp $1/*.gz $DATASET_FOLDER/files/$1/
    rm -rf $DATASET_FOLDER/$1
#done

