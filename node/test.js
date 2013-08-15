var Gumbo = require('./build/Release/gumbo');

var output = Gumbo.parse("<html><head></head><body></body></html>");

console.log(output);
