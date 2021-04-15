<?php
//Dao\Debug::enable();
Dao\Di::reset();
$router = new Dao\Mvc\Router(FALSE);

$router->add('/system/{controller}', ['controller' => 'controller']);


var_dump($router->handle('/system/hello'));
echo $router->getControllerName().PHP_EOL;
echo $router->getActionName().PHP_EOL;
var_dump($router->getParams());
Dao\Debug::disable();