<?php

/**
 * Detect content SLIDER and eventually create HTML.
 */
function wiki_SLIDESHOW($attributes, $value, $string, $page) {
  $dirlist = new Slider($page->env, $value, 'slider', array('sortbytime' => 'asc') + $attributes);
  return $dirlist->render();
}