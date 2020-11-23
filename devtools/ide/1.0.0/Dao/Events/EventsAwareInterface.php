<?php 

namespace Dao\Events {

	interface EventsAwareInterface {

		public function setEventsManager(\Dao\Events\ManagerInterface $eventsManager);


		public function getEventsManager();

	}
}
