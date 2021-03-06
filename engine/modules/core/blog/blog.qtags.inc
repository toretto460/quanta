<?php

/**
 * Implements BLOG qtag.
 *
 * Renders a "blog" view.
 *
 * TODO: pagination and other goodies.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $target
 *   The qtag's target.
 *
 * @param array $attributes
 *   The qtag's attributes.
 *
 * @return string
 *   The rendered qtag.
 */
function qtag_BLOG($env, $target, &$attributes) {
  $dirlist = new DirList($env, $target, 'blog', array('sort' => 'time') + $attributes, 'blog');
  return $dirlist->render();
}

/**
 * Implements READMORE qtag.
 *
 * Renders a "blog" view.
 *
 * TODO: pagination and other goodies.
 *
 * @param Environment $env
 *   The Environment.
 *
 * @param string $target
 *   The qtag's target.
 *
 * @param array $attributes
 *   The qtag's attributes.
 *
 * @return string
 *   The rendered qtag.
 */
function qtag_READMORE($env, $target, &$attributes) {
  return 'Read more';
}
