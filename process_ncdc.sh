DATASET_FOLDER="/Users/bgxavier/projects/OpenMP_MPI/dataset"

mkdir -p $DATASET_FOLDER/files/all

for year in {1901..1975}
do
  echo "Processing $year year"
  cd $DATASET_FOLDER/files/$year
  for i in `ls`
  do
    filename=`basename $i`
    gunzip -c $filename >> $year.all
  done
  gzip -c $year.all >> $year.gz
  mv $year.gz $DATASET_FOLDER/files/all/
  rm $year.all
done
