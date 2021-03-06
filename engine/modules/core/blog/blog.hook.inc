<?php
/**
 * Implements hook_load_includes().
 *
 * @param Environment $env
 *   The Environment.
 * @param $vars
 *   An array of variables.
 */
function blog_load_includes($env, $vars) {
  $module_path = $env->getModulePath('blog');

  $env->addInclude($module_path . '/css/blog.css');
}
