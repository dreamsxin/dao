<?php 

namespace Dao {

	abstract class Binary {

		const TYPE_CHAR = 0;

		const TYPE_UNSIGNEDCHAR = 1;

		const TYPE_INT16 = 2;

		const TYPE_UNSIGNEDINT16 = 3;

		const TYPE_INT = 4;

		const TYPE_UNSIGNEDINT = 5;

		const TYPE_INT32 = 6;

		const TYPE_UNSIGNEDINT32 = 7;

		const TYPE_FLOAT = 8;

		const TYPE_DOUBLE = 9;

		const ENDIAN_MACHINE = 0;

		const ENDIAN_BIG = 1;

		const ENDIAN_LITTLE = 2;

		public static function pack($type, $data, $endian=null){ }


		public static function unpack($type, $data, $endian=null){ }


		public static function setbit($data, $offset, $value){ }


		public static function getbit($data, $offset){ }


		public static function toBitstring($data){ }

	}
}
