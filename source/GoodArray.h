#ifndef FILE_GOODARRAY_SEEN
#define FILE_GOODARRAY_SEEN

typedef struct Idontevenknowwhatthisnameisfor {
	void* array;
	short width;
	short height;
	unsigned short singleElementSize;
} Good2dArray;

void InitGoodArray(Good2dArray* _array){
	_array->array=NULL;
	_array->width=0;
	_array->height=0;
	_array->singleElementSize=0;
	_array->array = calloc(1,1);
	if (_array->array==NULL){
		printf("Failed to init good array!\n");
	}
	//printf("Is now at %p\n",_array->array);
}

// _typeSize should be the size of one of that element. For example, a GoodArray of char I should pass sizeof(char)
void SetGoodArray(Good2dArray* _array, short _width, short _height, short _typeSize){
	_array->width=_width;
	_array->height=_height;
	//printf("Trying to realloc to %d at %p\n",_width*_height*_typeSize,_array->array);
	if (_array->array==NULL){
		InitGoodArray(_array);
	}
	_array->array = realloc((_array)->array, _width*_height*_typeSize);
	//_array->array = calloc(_width*_height,_typeSize);
	memset(_array->array,0,_width*_height*_typeSize);
	
	_array->singleElementSize=_typeSize;
}
char* GetGoodArray(Good2dArray* _array, short _x, short _y){
	return (((char*)((*_array).array))+(*_array).singleElementSize*(_x+(_y*(*_array).width)));
}

#endif
