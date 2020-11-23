<?php 

namespace Dao\Async\Network {

	final class TlsClientEncryption {

		public function __wakeup(){ }


		public function withAllowSelfSigned($allow){ }


		public function withVerifyDepth($depth){ }


		public function withPeerName($name){ }


		public function withAlpnProtocols($protocols){ }


		public function withCertificateAuthorityPath($path){ }


		public function withCertificateAuthorityFile($file){ }

	}
}
