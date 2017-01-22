#ifndef FILE_GOODARRAY_SEEN
#define FILE_GOODARRAY_SEEN

typedef struct Idontevenknowwhatthisnameisfor {
	void* array;
	short width;
	short height;
	unsigned short singleElementSize;
} Good2dArray;

void InitGoodArray(Good2dArray* _array){
	(*_array).array = malloc(1);
}

// _typeSize should be the size of one of that element. For example, a GoodArray of char I should pass sizeof(char)
void SetGoodArray(Good2dArray* _array, short _width, short _height, short _typeSize){
	(*_array).width=_width;
	(*_array).height=_height;
	(*_array).array = realloc((_array)->array, _width*_height*_typeSize);
	//(*_array).array = malloc(_width*_height*sizeof(char));
	(*_array).singleElementSize=_typeSize;
}
char* GetGoodArray(Good2dArray* _array, short _x, short _y){
	//printf("%d\n",(*_array).width);
	//return &((*_array).array[0]);
	//return &((*_array).array[0])
	//return &((*_array).array[_x+(_y*(*_array).width)]);
	return (((char*)((*_array).array))+(*_array).singleElementSize*(_x+(_y*(*_array).width)));
	//return 
}

#endif