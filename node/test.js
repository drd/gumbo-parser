var Gumbo = require('./build/Release/gumbo');

var output = Gumbo.parse("<html><head><title>Gumbo</title></head><body><p>Limbo</p></body></html>");

console.log(output);
