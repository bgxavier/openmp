#/usr/bin/bash
DATASET_FOLDER="/Users/bgxavier/projects/OpenMP_MPI/dataset"

for i in {1950..2013}
do
    cd $DATASET_FOLDER
    mkdir -p files/$i
    wget -r -np -nH --cut-dirs=3 -R index.html ftp://ftp.ncdc.noaa.gov/pub/data/noaa/$i/
    cd $i/
    cp *.gz $DATASET_FOLDER/files/$i/
    rm -rf $DATASET_FOLDER/$i
done

