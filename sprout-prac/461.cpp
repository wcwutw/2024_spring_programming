double distance(int x1, int y1, int x2, int y2) {
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool isValid(int a, int b, int c, int baba[], int mie[], int miao[], int x, int y) {
	bool ret=0;
	for(int i=0;i<a;i++) {
		double dist=distance(x, y, baba[2*i], baba[2*i+1]);
		if(dist<=10) ret=1;
	}
	if(!ret) return ret;
	ret=0;
	for(int i=0;i<b;i++) {
		double dist=distance(x, y, mie[2*i], mie[2*i+1]);
		if(dist<=10) ret=1;
	}
	if(!ret) return ret;
	ret=0;
	for(int i=0;i<c;i++) {
		double dist=distance(x, y, miao[2*i], miao[2*i+1]);
		if(dist<=12) ret=1;
	}
	return ret;
}
