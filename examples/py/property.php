<?php

Dao\Py::exec("
class Test:
    def __init__(self):
	    self.a = 1
");

$py = new Dao\Py\Object('__main__', 'Test');
var_dump(isset($py->a));
unset($py->a);
var_dump(isset($py->a));