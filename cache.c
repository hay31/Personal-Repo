/*스마트ict융합공학과 201814223 이하영 컴퓨터 구조 과제 2 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define SIZE 20

int solution(int cacheSize, char *x[], int length);
void display(int idx,int hit_miss,char city[][SIZE],int citiesSize,char cache[][SIZE],int cacheSize,int cashCount );
int findcache(int cityidx,char city[][SIZE],int citiesSize,char cache[][SIZE],int cacheSize );

int main(int argc, char *argv[]) //커맨드라인에 한번에 입력받음 ex) ./cache 3 jeju seoul busan LRU
{

    int cacheSize = 0;
    int runtimecount = 0;
    cacheSize = atoi(argv[1]);
    int length = argc-2;
    runtimecount = solution(cacheSize, &argv[2], length);
    printf("실행시간:");
    printf("%d\n", runtimecount);
    return 0;
}


int findcache(int cityidx,char city[][SIZE],int citiesSize,char cache[][SIZE],int cacheSize )
{

    for (int j = 0; j < cacheSize; j++) // 현재 cache안에 해당하는 city가 있다면 그 위치를 반환함
    {
        if (strcmp(cache[j],city[cityidx])==0)
        {
            return j;
        }
    }
    
    return -1;
}

void display(int idx,int hit_miss,char city[][SIZE],int citiesSize,char cache[][SIZE],int cacheSize,int cashCount ) //출력
{
    printf("단계: ");
    printf("%d\n", idx);
    printf("찾고자 하는 도시: ");
    printf("%s\n", city[idx]);
    printf("Cache hit 여부: ");
    if (hit_miss)
    {
        printf("Cache hit");
     
    }
    else
    {
        printf("Cache miss");
    
    }

    printf("\n현재 Cache의 상태:");

    for (int j = 0; j < cacheSize; j++)
    {
        printf("[%s]", cache[j]);
    }

    printf("\n\n");
}

int solution(int cacheSize, char *citiesandalgo[], int length)
{
    int citiesSize = length-1;
    char city[100][SIZE] = {
        0,
    };
    char algo[100] = {0};
    int runtimecount = 0;
    char cache[100][SIZE] = { 
        0,
    };
    
    int index_j = 0;
    int cashCount = 0; 
    int hit_miss;

    strcpy(algo, citiesandalgo[length - 1]); //마지막 인덱스는 알고리즘 이름(LRU, FIFO)이므로 algo에 저장한다.


    for (int i = 0; i < citiesSize; i++)
    {
        strcpy(city[i], citiesandalgo[i]); //매개변수로 전달받은 값을 city에 저장함.마지막 알고리즘 이름은 저장하지 않는다.        

        for (int j = 0; j < strlen(city[i]); j++)
        { //대문자 -> 소문자로 변경
             if (('A' <= city[i][j]) && ('Z' >= city[i][j]))
                    city[i][j] = city[i][j] - ('A'-'a');
        }
    }

    //캐시사이즈 0일 경우
    if (cacheSize == 0)
    {
        for (int i = 0; i < citiesSize; i++)
        {
            display(i,0,city,citiesSize,cache,cacheSize,cashCount);
            runtimecount+=5;
        }
        return runtimecount;
    }

    // LRU 알고리즘
    if (strcmp(algo, "LRU")==0)
    {
        for (int i = 0; i < citiesSize; i++)
        {
            index_j = findcache(i,city,citiesSize,cache,cacheSize); //사용자로부터 입력받은 city와 현재 cache를 비교하여 찾는 함수
            hit_miss=(index_j>=0); //cache 인덱스의 값과 city의 값이 일치할 경우 해당 cache의 해당 인덱스를 반환함
            if (hit_miss)  // cache에 값이 있을 경우(cache-hit)
            {
                for (int k = index_j; k < cacheSize; k++) // 해당 값 삭제함
                { 
                    strcpy(cache[k], cache[k + 1]);
                }
                runtimecount += 1;
                strcpy(cache[cacheSize - 1], city[i]); //다시 cache의 맨 마지막 행에 참조하려는 값을 추가하면서 최신의 상태로 변경시킴 .
					//LRU알고리즘이므로 최근에 사용한 페이지를 최신 아이템으로 update시켜주는 작업

            }
            else //cache에 값이 없을 경우( cache -miss)
            { 
                if (cashCount < cacheSize) // cachesize(사용자로부터 입력받은 값임)가 더 큰 경우 그대로 뒤에 추가하면 됨
                { 
                    strcpy(cache[cashCount], city[i]);
                    cashCount++;
                }
                else //cachesize가 더 작은경우  가장 오래된 값인 index가 0번째인 값을 삭제.가장 오랫동안 사용되지 않은 페이지를 교체함
                { 
                    for (int k = 0; k < cacheSize-1; k++)
                    { // 삭제 작업
                        strcpy(cache[k], cache[k + 1]);
                    }
                    strcpy(cache[cacheSize - 1], city[i]); // 맨 마지막행에 참조하려는 값을 추가하여 최신의 상태로 변경시킴 
                }
                runtimecount += 5;
            }

            //출력
            display(i,hit_miss,city,citiesSize,cache,cacheSize,cashCount);
        }
    }
    else if (strcmp(algo, "FIFO")==0)//FIFO 알고리즘
    { 
        for (int i = 0; i < citiesSize; i++)
        {
            index_j = findcache(i,city,citiesSize,cache,cacheSize);
            hit_miss=(index_j>=0);
            if (hit_miss) // cache에 값이 있을 경우(cache-hit) 
            { 
                runtimecount += 1;
            }
            else //cache에 값이 없을 경우( cache -miss)
            { 
                if (cashCount < cacheSize) // cachesize가 더 큼
                { 
                    strcpy(cache[cashCount], city[i]);
                    cashCount++;
                }
                else //cahcesize가 입력받은 city보다 사이즈가 작을 경우, FIFO알고리즘에 의해 인덱스 0번째의 값을 새로운 값으로 교체한다.
                {
        
                    for (int k = cacheSize-1; k >0; k--)// 가장 오래된 값 삭제 index =0
                    { 
                        strcpy(cache[k], cache[k - 1]);
                    }
                    strcpy(cache[0], city[i]);
                }

                runtimecount += 5;
            }
            //출력
            display(i,hit_miss,city,citiesSize,cache,cacheSize,cashCount);
        }
    }	
    else { //LRU,FIFO가 아닌 다른 문자열이 입력되었을 때

	printf("LRU,FIFO중에 하나를 입력해주세요.\n");
	return 0;


	}
    
    return runtimecount;
}