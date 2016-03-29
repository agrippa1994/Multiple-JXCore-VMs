var vm = require("vm");

(function() {
    console.log("hallo");
})();


setInterval(function() {
    console.log(vm.getThreadId());
    ASD
}, 1000);