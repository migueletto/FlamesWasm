// memory shared between WASM and JavaScript
var wasmMemory = new WebAssembly.Memory({
  "initial": 256,
  "maximum": 256
});

// ImageData requires a Uint8ClampedArray
HEAP = new Uint8ClampedArray(wasmMemory.buffer);

// retrieve the context to draw on the HTML canvas
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

// create an ImageData to hold the image
const image = new ImageData(canvas.width, canvas.height);
const len = canvas.width * canvas.height * 4;

var anim;
var last_ts = 0;

function draw(ts) {
  // reduce the frame rate a bit
  if ((ts - last_ts) > 100.0) {
    // call WASM anim() function
    const addr = anim();

    // the returned value is the image buffer address
    // put the buffer inside an array slice
    const buffer = HEAP.slice(addr, addr + len);

    // update the image data with the buffer contents
    image.data.set(buffer);

    // draw the image on the canvas
    ctx.putImageData(image, 0, 0);

    last_ts = ts;
  }

  // request the next frame
  requestAnimationFrame(draw);
}

// the environment for the WASM runtime
var env = {
  "memory": wasmMemory
}

// load and run the WASM code in "flame.wasm"
WebAssembly.instantiateStreaming(
    fetch("flame.wasm"),
    { env: env, wasi_snapshot_preview1: env }
  ).then(
  (obj) => {
    // call WASM init() function once
    obj.instance.exports.init();

    // save WASM function anim() to be called later
    anim = obj.instance.exports.anim;

    // request to call JavaScript draw() function on the next frame
    requestAnimationFrame(draw);
  }
);
