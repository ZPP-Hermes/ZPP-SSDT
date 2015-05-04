/* parametry skryptu (17)
 * 1 - liczba wygenerowanych zestawów ocen (plus wyborów seminariów)
 * 2-8 - procentowy średni rozkład ocen (od 2 do 5!), ma się sumować do 100
 * 9-15 - procentowy odchył od tego rozkładu (dla grupy docelowej i pobliskich),
 * ma się sumować do 0 (ujemne np. dla słabych ocen) najlepiej 
 * jakby moduły poszczególnych procentów nie przekraczały 9/11 odpowiedniego średniego
 * 16 - średni procent wybranych obieraków
 * 17 - odchył tego wyboru (żeby grupa docelowa była częściej wybierana)
 * */



#include <stdio.h>
#include <stdlib.h>
	int wyrownanie[10]={45,37,31,27,25,25,27,31,37,45};
	int procenty[7], odchyly[7];
	int wybieralnosc,wyb_odch;

float modul(int k, int l){
	int m = (k<l?l-k:k-l);
	return 1.0-(10.0*m)/wyrownanie[k];
}

int choose(int k, int l){
	int r=rand()%100;
	if(r<wybieralnosc+wyb_odch*modul(k,l)){
		return 0;
	}else{
		return 1;
	}
}

int rank(int k, int l){
	float m = modul(k,l);
	int r=rand()%100;
	if(r<procenty[0]+m*odchyly[0]){
		return 4;
	}else if(r<procenty[1]+m*odchyly[1]){
		return 6;
	}else if(r<procenty[2]+m*odchyly[2]){
		return 7;
	}else if(r<procenty[3]+m*odchyly[3]){
		return 8;
	}else if(r<procenty[4]+m*odchyly[4]){
		return 9;
	}else if(r<procenty[5]+m*odchyly[5]){
		return 10;
	}else{
		return 11;
	}
}


void main(int argc, char *argv[]){
	int i,j,k,n,r;
	
	if(argc!=18){
		fprintf(stderr, "zła liczba argumentów\n");
		return;
	}
	n=atoi(argv[1]);
	/*wczytanie parametrów i sprawdzenie warunków*/
	procenty[0]=atoi(argv[2]);
	for(i=1;i<7;++i){
		procenty[i]=procenty[i-1]+atoi(argv[i+2]);
	}
	if(procenty[6]!=100){
		fprintf(stderr, "procenty powinny się sumować do 100\n");
		return;
	}
	odchyly[0]=atoi(argv[9]);
	for(i=1;i<7;++i){
		odchyly[i]=odchyly[i-1]+atoi(argv[i+9]);
	}
	if(odchyly[6]!=0){
		fprintf(stderr, "odchyły powinny się sumować do 0\n");
		return;
	}
	wybieralnosc=atoi(argv[16]);
	wyb_odch=atoi(argv[17]);
	//TODO sprawdzić nie wychodzenie poza zakres procentów (a nawet 9/11)
	/*generowanie ocen*/
	srand(time(NULL));
	for(i=0;i<n;++i){
		k=rand()%10;
		for(j=0;j<30;++j){
			printf("%d,",rank(k,j/3));
		}
		for(j=0;j<20;++j){
			printf("%d,",(choose(k,j/2)?rank(k,j/2):0));
		}
		printf("%d\n",k);
	}

}
