/* parametry skryptu (18)
 * 1 - liczba wygenerowanych zestawów ocen (plus wyborów seminariów)
 * 2 - offset (żeby pk studentów zaczynały się dalej)
 * 3-9 - procentowy średni rozkład ocen (od 2 do 5!), ma się sumować do 100
 * 10-16 - procentowy odchył od tego rozkładu (dla grupy docelowej i pobliskich),
 * ma się sumować do 0 (ujemne np. dla słabych ocen) najlepiej 
 * jakby moduły poszczególnych procentów nie przekraczały 9/11 odpowiedniego średniego
 * 17 - średni procent wybranych obieraków
 * 18 - odchył tego wyboru (żeby grupa docelowa była częściej wybierana)
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
		return 1;
	}else{
		return 0;
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
	int i,j,k,n,r,off;
	
	if(argc!=19){
		fprintf(stderr, "zła liczba argumentów\n");
		return;
	}
	n=atoi(argv[1]);
	off=atoi(argv[2]);
	/*wczytanie parametrów i sprawdzenie warunków*/
	procenty[0]=atoi(argv[3]);
	for(i=1;i<7;++i){
		procenty[i]=procenty[i-1]+atoi(argv[i+3]);
	}
	if(procenty[6]!=100){
		fprintf(stderr, "procenty powinny się sumować do 100\n");
		return;
	}
	odchyly[0]=atoi(argv[10]);
	for(i=1;i<7;++i){
		odchyly[i]=odchyly[i-1]+atoi(argv[i+10]);
	}
	if(odchyly[6]!=0){
		fprintf(stderr, "odchyły powinny się sumować do 0\n");
		return;
	}
	wybieralnosc=atoi(argv[17]);
	wyb_odch=atoi(argv[18]);
	//TODO sprawdzić nie wychodzenie poza zakres procentów (a nawet 9/11)
	/*generowanie ocen*/
	srand(time(NULL));
	printf("[");
	for(i=0;i<30;++i){
		printf("{\"model\":\"ZPP1_forscript.course\",\"pk\" : %d, \"fields\": {\"name\":\"obow%d\", \"type\": \"OBOW\"}},\n",i+1,i+1);
	}
	for(i=0;i<20;++i){
		printf("{\"model\":\"ZPP1_forscript.course\",\"pk\" : %d, \"fields\": {\"name\":\"obier%d\", \"type\": \"OBIER\"}},\n",i+31,i+31);
	}
	for(i=0;i<10;++i){
		printf("{\"model\":\"ZPP1_forscript.course\",\"pk\" : %d, \"fields\": {\"name\":\"sem%d\", \"type\": \"SEM\"}},\n",i+51,i+51);
	}
	for(i=off;i<n+off;++i){
		printf("{\"model\":\"ZPP1_forscript.student\",\"pk\" : %d, \"fields\": {\"usos_id\":\"%d\"}},\n",i+1,i+1);
		k=rand()%10;
		for(j=0;j<30;++j){
			printf("{\"model\":\"ZPP1_forscript.mark\", \"fields\": {\"student\":%d, \"course\":%d, \"mark\": \"%d\"}},\n",i+1,j+1,rank(k,j/3));
		}
		for(j=0;j<20;++j){
			if(choose(k,j/2)!=0){
				printf("{\"model\":\"ZPP1_forscript.mark\", \"fields\": {\"student\":%d, \"course\":%d, \"mark\": \"%d\"}},\n",i+1,j+31,rank(k,j/2));
			}
		}
		printf("{\"model\":\"ZPP1_forscript.mark\", \"fields\": {\"student\":%d, \"course\":%d, \"mark\": \"%d\"}},\n",i+1,k+51,rank(k,k));
	}
	printf("{\"model\":\"ZPP1_forscript.student\",\"pk\" : %d, \"fields\": {\"usos_id\":\"%d\"}}\n",n+1,n+1);
	printf("]");

}
