<?php 

namespace Dao {

	interface EscaperInterface {

		public function setEncoding($encoding);


		public function getEncoding();


		public function setHtmlQuoteType($quoteType);


		public function escapeHtml($text);


		public function escapeHtmlAttr($attr);


		public function escapeCss($css);


		public function escapeJs($js);


		public function escapeUrl($url);

	}
}
