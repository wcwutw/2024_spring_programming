int range(int array[], int size) {
	int mx=-1, mn=101;
	for(int i=0;i<size;i++) {
		if(array[i]>mx) mx=array[i];
		if(array[i]<mn) mn=array[i];
	}
	return mx-mn;
}
double average(int array[], int size) {
	double sum=0.0;
	for(int i=0;i<size;i++) {
		sum=sum+(double)array[i];
	}
	sum=sum/(double)size;
	return sum;
}
