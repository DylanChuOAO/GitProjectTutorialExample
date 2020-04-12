#include "Queue.h"

void newMen(int **arr) {
	arr = new int*[rowN];
	for (int i = 0; i < rowN; i++)
		arr[i] = new int[rowN];
}

void printArray(int **arr) {
	printf("[");
	for (int j = 0; j < rowN; j++)
		for (int k = 0; k < rowN; k++)
			if (j == 0 && k == 0)
				printf("%d", arr[j][k]);
			else
				printf(" %d", arr[j][k]);
	printf("]\n");
}

int heuristic(int **arr) {
	int count = 0;
	for (int i = 0; i < rowN; i++)
		for (int j = 0; j < rowN; j++)
			if (arr[i][j] != 0)
				count += abs(i - arr[i][j] / rowN) + abs(j - arr[i][j] % rowN);
	return count;
}

bool unsolvable(int **arr) {
	int count = 0;
	for (int j = 0; j < rowN*rowN; j++)
		for (int k = j + 1; k < rowN*rowN; k++)
			if (arr[j / rowN][j%rowN] != 0 && arr[k / rowN][k%rowN] != 0)
				if (arr[j / rowN][j%rowN] > arr[k / rowN][k%rowN])
					count++;
	if (count % 2 == rowN % 2)
		return true;
	else
		return false;
}


void swap(int** arr, int i1, int j1, int i2, int j2) {
	int temp = arr[i1][j1];
	arr[i1][j1] = arr[i2][j2];
	arr[i2][j2] = temp;
}

int **newArr(int **src){
	int **newArray;
	newArray= new int*[rowN];
	for (int i = 0; i < rowN; i++)
		newArray[i] = new int[rowN];
	for (int i = 0; i < rowN; i++)
		for (int j = 0; j < rowN; j++)
			newArray[i][j] = src[i][j];
	return newArray;
}

List *findSuccessor(int **arr) {
	int i, j, count = 0;
	for (int index = 0; index < rowN*rowN; index++)
		if (arr[index / 3][index % 3] == 0) {
			i = index / 3;
			j = index % 3;
			break;
		}
	count += (i - 1 >= 0) ? 1 : 0;
	count += (j - 1 >= 0) ? 1 : 0;
	count += (i + 1 < rowN) ? 1 : 0;
	count += (j + 1 < rowN) ? 1 : 0;
	List *list = new List[4];
	if (i - 1 >= 0) {//上
		list[0].arr = newArr(arr);
		list[0].action = UP;
		swap(list[0].arr, i, j, i - 1, j);
	}
	if (i + 1 < rowN) {//下
		list[1].arr = newArr(arr);
		list[1].action = DOWN;
		swap(list[1].arr, i, j, i + 1, j);
	}
	if (j - 1 >= 0) {//左
		list[2].arr = newArr(arr);
		list[2].action = LEFT;
		swap(list[2].arr, i, j, i, j - 1);
	}
	if (j + 1 < rowN) {//右
		list[3].arr = newArr(arr);
		list[3].action = RIGHT;
		swap(list[3].arr, i, j, i, j + 1);
	}
	return list;
}

Data* reverseData(Data* data) {
	Data *current = data;
	Data *rear = NULL;
	Data *front = data->parent;
	current->parent = rear;
	while (front != NULL) {
		rear = current;
		current = front;
		front = front->parent;
		current->parent = rear;
	}
	return current;
}

void traverseData(Data* data) {
	while (data != NULL) {
		if (data->action == UP) {//上
			printf("move 0 to up\n");
		}
		if (data->action == DOWN) {//下
			printf("move 0 to down\n");
		}
		if (data->action == LEFT) {//左
			printf("move 0 to left\n");
		}
		if (data->action == RIGHT) {//右
			printf("move 0 to right\n");
		}
		data = data->parent;
	}
}

int main() {
	Queue *queue = new Queue();
	Queue *top = NULL;
	Data *newData = new Data;
	Data *finalData = NULL;
	newData->arr = new int*[rowN];
	for (int i = 0; i < rowN; i++)
		newData->arr[i] = new int[rowN];
	scanf(" %*c");
	for (int j = 0; j < rowN; j++)
		for (int k = 0; k < rowN; k++)
			scanf(" %d", &newData->arr[j][k]);
	scanf(" %*c");
	if (unsolvable(newData->arr)) {
		printf("No solution!!\n");
		return 0;
	}
	newData->g = 0;
	newData->h = heuristic(newData->arr);
	newData->f = newData->g + newData->h;
	newData->parent = NULL;
	if (newData->h == 0) {
		printf("It is the goal state.\n");
		return 0;
	}
	queue->enqueue(top, newData);
	while (!queue->isEmpty(top)) {
		Data *dequeueData = queue->dequeue(top);
		if (dequeueData->h == 0) {
			finalData = dequeueData;
			break;
		}
		List *list = findSuccessor(dequeueData->arr);
		for (int i = 0; i < 4; i++) {
			if (list[i].action == 0)
				continue;
			Data *nextData = new Data;
			nextData->parent = dequeueData;
			nextData->arr = list[i].arr;
			nextData->action = list[i].action;
			nextData->h = heuristic(list[i].arr);
			nextData->g = dequeueData->g + 1;
			nextData->f = nextData->g + nextData->h;
			queue->enqueue(top, nextData);
		}
	}
	if (finalData == NULL)
		printf("No solution!!\n");
	else {
		finalData = reverseData(finalData);
		traverseData(finalData);
	}
		
	system("pause");
	return 0;
}