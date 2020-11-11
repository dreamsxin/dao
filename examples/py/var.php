<?php

$global = 'global';

$py = <<<EOT
import phalcon

var = phalcon.var('global')

unknown = ''
try:
    phalcon.var('unknown')
except NameError, e:
    unknown = str(e)
EOT;

Dao\Py::exec($py);
var_dump(Dao\Py::eval('var'));
var_dump(Dao\Py::eval('unknown'));