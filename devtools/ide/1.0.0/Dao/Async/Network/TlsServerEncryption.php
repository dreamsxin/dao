<?php 

namespace Dao\Async\Network {

	final class TlsServerEncryption {

		public function __wakeup(){ }


		public function withDefaultCertificate($cert, $key=null, $passphrase=null){ }


		public function withCertificate($host, $cert, $key=null, $passphrase=null){ }


		public function withAlpnProtocols($protocols){ }


		public function withCertificateAuthorityPath($file){ }


		public function withCertificateAuthorityFile($file){ }

	}
}
