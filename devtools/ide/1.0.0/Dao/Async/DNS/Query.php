<?php 

namespace Dao\Async\DNS {

	final class Query {

		const A = 1;

		const A6 = 38;

		const AAAA = 28;

		const ANY = 255;

		const CAA = 257;

		const CNAME = 5;

		const HINFO = 13;

		const MX = 15;

		const NAPTR = 35;

		const NS = 2;

		const PTR = 12;

		const SOA = 6;

		const SRV = 33;

		const TXT = 16;

		public $host;

		public function __construct($host, $types){ }


		public function __wakeup(){ }


		public function getTypes(){ }


		public function getRecords(){ }


		public function addRecord($type, $ttl, $data){ }

	}
}
