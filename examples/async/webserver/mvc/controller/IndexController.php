<?php

class IndexController extends \Dao\Mvc\Controller {

	public function indexAction() {
		return 'Hello world';
	}

	public function viewAction() {
		echo 'view -';
	}
}