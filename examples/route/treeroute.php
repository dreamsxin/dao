<?php
Dao\Debug::enable();
Dao\Di::reset();

$router = new Dao\Mvc\Router(FALSE);
$router->add('/system/{controller}/{action}');
var_dump($router->handle('/system/hello'));
var_dump($router->getControllerName(), $router->getActionName(), $router->getParams());

$router = new Dao\Mvc\Router(FALSE);
$router->add('/system/:controller/:action/:params');
var_dump($router->handle('/system/hello/world/dao'));
var_dump($router->getControllerName(), $router->getActionName(), $router->getParams());

Dao\Debug::disable();