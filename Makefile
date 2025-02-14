flame.wasm: flame.c
	emcc -o flame.wasm --no-entry -s IMPORTED_MEMORY -s EXPORTED_FUNCTIONS=_init,_anim flame.c
