#include "shared.h++"

#include <stddef.h>

#include "random.h++"
#include "chunk.h++"

int main(){
	Rand *r=new randchance(0.2);
	chunk<bool> c(5,5,5);
	size_t i,j,k;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				c[i][j][k]=(*r)();
			}
		}
	}

	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				printf("%i",c[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}