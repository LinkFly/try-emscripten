const factory = require('./node.js');

factory().then((instance) => {
    instance._sayHi();
    instance.ccall('sayHi');
    console.log(instance._daysInWeek());
});

