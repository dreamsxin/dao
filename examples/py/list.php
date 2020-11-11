<?php

$list = Dao\Py::list([5,1,2,3,4]);
var_dump($list);
var_dump(Dao\Py::callMethod($list, 'sort'));
var_dump($list);