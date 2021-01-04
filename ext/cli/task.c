
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  |          Rack Lin <racklin@gmail.com>                                  |
  +------------------------------------------------------------------------+
*/

#include "cli/task.h"
#include "di/injectable.h"

#include "kernel/main.h"

/**
 * Dao\Cli\Task
 *
 * Every command-line task should extend this class that encapsulates all the task functionality
 *
 * A task can be used to run "tasks" such as migrations, cronjobs, unit-tests, or anything that you want.
 * The Task class should at least have a "mainAction" method
 *
 *<code>
 *
 *class HelloTask extends \Dao\Cli\Task
 *{
 *
 *  //This action will be executed by default
 *  public function mainAction()
 *  {
 *
 *  }
 *
 *  public function findAction()
 *  {
 *
 *  }
 *
 *}
 *
 *</code>
 */
zend_class_entry *dao_cli_task_ce;

static const zend_function_entry dao_cli_task_method_entry[] = {
	PHP_FE_END
};



/**
 * Dao\Cli\Task initializer
 */
DAO_INIT_CLASS(Dao_Cli_Task){

	DAO_REGISTER_CLASS_EX(Dao\\Cli, Task, cli_task, dao_di_injectable_ce, dao_cli_task_method_entry, 0);

	return SUCCESS;
}
