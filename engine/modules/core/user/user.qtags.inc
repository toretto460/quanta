<?php
// TODO: redo the crap!
/**
 * Implements qtag USERATTRIBUTE.
 *
 * Returns an user attribute of a given user.
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
function qtag_USERATTRIBUTE($env, $target, &$attributes) {
  $user = ($target == NULL) ? UserFactory::current($env) : new User($env, $target);
  switch ($attributes['name']) {

    // User's login name.
    case 'username':
      $string = $user->getName();
      break;
    // User's last name.
    case 'last_name':
      $string = $user->getLastName();
      break;
    // User's email.
    case 'email':
      $string = $user->getEmail();
      break;

    // User's first name.
    case 'first_name':
      $string = $user->getFirstName();
      break;

    default:
      $string = qtag_ATTRIBUTE($env, $user->getName(), $attributes);
      break;
  }
  return $string;
}

/**
 * Implements qtag LOGIN.
 *
 * Renders an user login link.
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
function qtag_LOGIN($env, $target, &$attributes) {
  $user = UserFactory::current($env);
  if ($user->exists) {
    if (!isset($attributes['link_class'])) {
      $attributes['link_class'] = 'logout-link';
    }
    $attributes['title'] = isset($attributes['title']) ? $attributes['title'] : t('Logout');
  }
  else {
    if (!isset($attributes['link_class'])) {
      $attributes['link_class'] = 'login-link';
    }
    $attributes['title'] = isset($attributes['title']) ? $attributes['title'] : t('Login');
  }
  $link = qtag_LINK($env, $target, $attributes);

  return $link;
}

/**
 * Implements qtag REGISTER.
 *
 * Renders an user sign up / registration link.
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
function qtag_REGISTER($env, $target, &$attributes) {
  $user = UserFactory::current($env);
  if (UserAccess::check($env, USER_ACTION_REGISTER, array('user' => $user))) {
    $title = isset($attributes['title']) ? $attributes['title'] : 'Sign up';
    return $user->exists ? '' : '[LINK|title=' . $title . '|link_class=register-link]';
  }
  else {
    return '';
  }
}

/**
 * Implements qtag USER_EDIT.
 *
 * Renders a link to edit an user profile.
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
function qtag_USER_EDIT($env, $target, &$attributes) {
  $user = ($target == NULL) ? UserFactory::current($env) : new User($env, $target);
  if (UserAccess::check($env, USER_ACTION_EDIT, array('user' => $user))) {
    $title = isset($attributes['title']) ? $attributes['title'] : 'Edit profile';
    return $user->exists ? '[LINK|title=' . $title . '|link_class=user-edit-link]' : $target;
  }
  else {
    return '';
  }
}

/**
 * Implements qtag USER_EDIT.
 *
 * Renders a link to edit an user profile.
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
function qtag_USER_EDIT_OWN($env, $target, &$attributes) {
  $user = ($target == NULL) ? UserFactory::current($env) : new User($env, $target);
  if (UserAccess::check($env, USER_ACTION_EDIT, array('user' => $user))) {
    $title = isset($attributes['title']) ? $attributes['title'] : 'Edit your profile';
    return $user->exists ? '[LINK|title=' . $title . '|link_class=user-edit-own-link]' : $target;
  }
  else {
    return '';
  }
}