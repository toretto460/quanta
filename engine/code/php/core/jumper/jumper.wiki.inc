<?php

/**
 * Detect content JUMPER and eventually create HTML.
 */
function wiki_JUMPER($attributes, $value, $string, $env) {

  $dirlist = new Jumper($env, $value, 'jumper', array('sortbytime' => 'asc') + $attributes);
  $ajax = (isset($attributes['ajax'])) ? $attributes['ajax'] : '_self';
  $empty = (isset($attributes['empty'])) ? $attributes['empty'] : '----------';
  $tpl = (isset($attributes['tpl'])) ? ('tpl="' . $attributes['tpl'] . '"') : '';

  $jumper = '<select class="jumper" rel="' . $ajax . '" ' . $tpl . '><option value="' . JUMPER_EMPTY . '">' . $empty . '</option>' . $dirlist->render() . '</select>';
  return $jumper;
}