/*
 * libosinfo:
 *
 * Copyright (C) 2009-2012 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *   Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <osinfo/osinfo.h>
#include <glib/gi18n-lib.h>

G_DEFINE_TYPE (OsinfoInstallConfig, osinfo_install_config, OSINFO_TYPE_ENTITY);

#define OSINFO_INSTALL_CONFIG_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), OSINFO_TYPE_INSTALL_CONFIG, OsinfoInstallConfigPrivate))

/**
 * SECTION:osinfo_install_config
 * @short_description: OS install configuration
 * @see_also: #OsinfoInstallScript
 *
 * #OsinfoInstallConfig is an object for representing OS
 * install configuration data. It is used to generate an
 * automated installation script
 */

struct _OsinfoInstallConfigPrivate
{
    gboolean unused;
};


/* Init functions */
static void
osinfo_install_config_class_init (OsinfoInstallConfigClass *klass)
{
    g_type_class_add_private (klass, sizeof (OsinfoInstallConfigPrivate));
}

static const gchar valid[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'X', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_', '+',
    '=', '!', '@', '#', '%', '^', '&', ',', '(', ')', '[', '{', '}',
    '[', ']', ';', ':', '<', '>', ',', '.', '?', '/', '~',
};


static void
osinfo_install_config_init (OsinfoInstallConfig *config)
{
    gchar pass[9];
    gsize i;

    config->priv = OSINFO_INSTALL_CONFIG_GET_PRIVATE(config);

    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_KEYBOARD,
                            "en_US");
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_TIMEZONE,
                            "America/New_York");
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_LANGUAGE,
                            "en_US");

    for (i = 0 ; i < sizeof(pass)-1 ; i++) {
        gint val = g_random_int_range(0, sizeof(valid));
        pass[i] = valid[val];
    }
    pass[sizeof(pass)-1] = '\0';

    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_ADMIN_PASSWORD,
                            pass);
}


/**
 * osinfo_install_config_new:
 * @id: the unique identifier
 *
 * Construct a new install configuration with default values for
 * language, keyboard, timezone and admin password. The default values
 * are to use an 'en_US' language and keyboard, and an 'America/New_York'
 * timezone. The admin password is set to a random 8 character password.
 *
 * Returns: (transfer full): an install configuration with default
 * values
 *
 */
OsinfoInstallConfig *osinfo_install_config_new(const gchar *id)
{
    return g_object_new(OSINFO_TYPE_INSTALL_CONFIG,
                        "id", id,
                        NULL);
}

void osinfo_install_config_set_hardware_arch(OsinfoInstallConfig *config,
                                             const gchar *arch)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_HARDWARE_ARCH,
                            arch);
}


const gchar *osinfo_install_config_get_hardware_arch(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_HARDWARE_ARCH);
}

/**
 * osinfo_install_config_set_l10n_keyboard:
 * @config: the install config
 * @keyboard: the keyboard
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_L10N_KEYBOARD parameter.
 *
 * The expected format of this string is the same as
 * #osinfo_install_config_set_l10n_language function's 'language' parameter.
 */
void osinfo_install_config_set_l10n_keyboard(OsinfoInstallConfig *config,
                                             const gchar *keyboard)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_KEYBOARD,
                            keyboard);
}


const gchar *osinfo_install_config_get_l10n_keyboard(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_L10N_KEYBOARD);
}

/**
 * osinfo_install_config_set_l10n_language:
 * @config: the install config
 * @language: the language
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_L10N_LANGUAGE parameter.
 *
 * The expected format of this string is the gettext locale names standard:
 *
 * https://www.gnu.org/savannah-checkouts/gnu/gettext/manual/html_node/Locale-Names.html
 *
 * Encoding and variant are (at least for now) not supported. For example,
 * 'pt_BR' is accepted is accepted as the language codes for Brazilian Portuguese
 * but 'pt_BR.utf8' is not.
 */
void osinfo_install_config_set_l10n_language(OsinfoInstallConfig *config,
                                             const gchar *language)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_LANGUAGE,
                            language);
}


const gchar *osinfo_install_config_get_l10n_language(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_L10N_LANGUAGE);
}


void osinfo_install_config_set_l10n_timezone(OsinfoInstallConfig *config,
                                             const gchar *tz)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_L10N_TIMEZONE,
                            tz);
}


const gchar *osinfo_install_config_get_l10n_timezone(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_L10N_TIMEZONE);
}


void osinfo_install_config_set_admin_password(OsinfoInstallConfig *config,
                                              const gchar *password)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_ADMIN_PASSWORD,
                            password);
}


const gchar *osinfo_install_config_get_admin_password(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_ADMIN_PASSWORD);
}


void osinfo_install_config_set_user_login(OsinfoInstallConfig *config,
                                          const gchar *username)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_USER_LOGIN,
                            username);
}


const gchar *osinfo_install_config_get_user_login(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_USER_LOGIN);
}



void osinfo_install_config_set_user_password(OsinfoInstallConfig *config,
                                             const gchar *password)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_USER_PASSWORD,
                            password);
}


const gchar *osinfo_install_config_get_user_password(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_USER_PASSWORD);
}


void osinfo_install_config_set_user_realname(OsinfoInstallConfig *config,
                                             const gchar *name)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_USER_REALNAME,
                            name);
}


const gchar *osinfo_install_config_get_user_realname(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_USER_REALNAME);
}



void osinfo_install_config_set_user_autologin(OsinfoInstallConfig *config,
                                              gboolean autologin)
{
    osinfo_entity_set_param_boolean(OSINFO_ENTITY(config),
                                    OSINFO_INSTALL_CONFIG_PROP_USER_AUTOLOGIN,
                                    autologin);
}


gboolean osinfo_install_config_get_user_autologin(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value_boolean(OSINFO_ENTITY(config),
                                                 OSINFO_INSTALL_CONFIG_PROP_USER_AUTOLOGIN);
}


void osinfo_install_config_set_user_administrator(OsinfoInstallConfig *config,
                                                  gboolean admin)
{
    osinfo_entity_set_param_boolean(OSINFO_ENTITY(config),
                                    OSINFO_INSTALL_CONFIG_PROP_USER_ADMIN,
                                    admin);
}


gboolean osinfo_install_config_get_user_administrator(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value_boolean(OSINFO_ENTITY(config),
                                                 OSINFO_INSTALL_CONFIG_PROP_USER_ADMIN);
}


void osinfo_install_config_set_reg_login(OsinfoInstallConfig *config,
                                         const gchar *name)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_REG_LOGIN,
                            name);
}

const gchar *osinfo_install_config_get_reg_login(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_REG_LOGIN);
}


void osinfo_install_config_set_reg_password(OsinfoInstallConfig *config,
                                            const gchar *password)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_REG_PASSWORD,
                            password);
}

const gchar *osinfo_install_config_get_reg_password(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_REG_PASSWORD);
}


void osinfo_install_config_set_reg_product_key(OsinfoInstallConfig *config,
                                               const gchar *key)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_REG_PRODUCTKEY,
                            key);
}

const gchar *osinfo_install_config_get_reg_product_key(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_REG_PRODUCTKEY);
}

void osinfo_install_config_set_hostname(OsinfoInstallConfig *config,
                                        const gchar *hostname)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_HOSTNAME,
                            hostname);
}

const gchar *osinfo_install_config_get_hostname(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_HOSTNAME);
}

/**
 * osinfo_install_config_set_target_disk:
 * @config: the install config
 * @disk: the target disk
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_TARGET_DISK parameter.
 *
 * Note that the format of this string is dependent on the installer script
 * @config is going to be used with. You can use
 * #osinfo_install_script_get_path_format() to find out which format
 * does the script expects this string to be in. In case of
 * #OSINFO_PATH_FORMAT_UNIX unix device node names are expected, e.g "/dev/fd0".
 * In case of #OSINFO_PATH_FORMAT_DOS drive letters are expected, e.g "A".
 */
void osinfo_install_config_set_target_disk(OsinfoInstallConfig *config,
                                           const gchar *disk)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_TARGET_DISK,
                            disk);
}

/**
 * osinfo_install_config_get_target_disk:
 * @config: the install config
 *
 * Returns: The value of #OSINFO_INSTALL_CONFIG_PROP_TARGET_DISK parameter,
 *          or NULL.
 */
const gchar *osinfo_install_config_get_target_disk(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_TARGET_DISK);
}

/**
 * osinfo_install_config_set_script_disk:
 * @config: the install config
 * @disk: the disk
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_SCRIPT_DISK parameter.
 *
 * Please read documentation on #osinfo_install_config_set_target_disk() for
 * explanation on the format of @disk string.
 */
void osinfo_install_config_set_script_disk(OsinfoInstallConfig *config,
                                           const gchar *disk)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_SCRIPT_DISK,
                            disk);
}

/**
 * osinfo_install_config_get_script_disk:
 * @config: the install config
 *
 * Returns: The value of #OSINFO_INSTALL_CONFIG_PROP_SCRIPT_DISK parameter,
 *          or NULL.
 */
const gchar *osinfo_install_config_get_script_disk(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_SCRIPT_DISK);
}

/**
 * osinfo_install_config_set_avatar_location:
 * @config: the install config
 * @location: new location
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_AVATAR_LOCATION parameter.
 *
 * Note that the format of this string is dependent on the installer script
 * @config is going to be used with. You can use
 * #osinfo_install_script_get_path_format() to find out which format
 * does the script expects this string to be in.
 *
 * Also note that in case of #OSINFO_PATH_FORMAT_DOS, the drive/disk letter
 * and the leading ':' must not be included in the path.
 */
void osinfo_install_config_set_avatar_location(OsinfoInstallConfig *config,
                                               const gchar *location)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_AVATAR_LOCATION,
                            location);
}

/**
 * osinfo_install_config_get_avatar_location:
 * @config: the install config
 *
 * Returns: The value of #OSINFO_INSTALL_CONFIG_PROP_AVATAR_LOCATION parameter,
 *          or NULL.
 */
const gchar *osinfo_install_config_get_avatar_location(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_AVATAR_LOCATION);
}

/**
 * osinfo_install_config_set_avatar_disk:
 * @config: the install config
 * @disk: the avatar disk
 *
 * Sets the #OSINFO_INSTALL_CONFIG_PROP_AVATAR_DISK parameter.
 *
 * Please read documentation on #osinfo_install_config_set_target_disk() for
 * explanation on the format of @disk string.
 */
void osinfo_install_config_set_avatar_disk(OsinfoInstallConfig *config,
                                           const gchar *disk)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_AVATAR_DISK,
                            disk);
}

/**
 * osinfo_install_config_get_avatar_disk:
 * @config: the install config
 *
 * Returns: The value of #OSINFO_INSTALL_CONFIG_PROP_AVATAR_DISK parameter,
 *          or NULL.
 */
const gchar *osinfo_install_config_get_avatar_disk(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config),
                                         OSINFO_INSTALL_CONFIG_PROP_AVATAR_DISK);
}

/**
 * osinfo_install_config_set_pre_install_drivers_disk:
 * @config: the install config
 * @disk: the disk
 *
 * Specify the disk on which drivers to be installed at the very beginning of
 * installation, are available. This is usually needed for devices for which the
 * OS in question does not have out of the box support for and devices are
 * required/prefered to be available during actual installation.
 *
 * Please read documentation on #osinfo_install_config_set_target_disk() for
 * explanation on the format of @disk string.
 *
 * NOTE: Not every install script supports pre-installation of drivers. Use
 * #osinfo_install_script_get_can_pre_install_drivers() to find out if an
 * installer script supports it.
 *
 * NOTE: Microsoft Windows XP requires pre-installation driver files to be
 * present in the script disk under the toplevel directory.
 */
void osinfo_install_config_set_pre_install_drivers_disk(OsinfoInstallConfig *config,
                                                        const gchar *disk)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_PRE_INSTALL_DRIVERS_DISK,
                            disk);
}

/**
 * osinfo_install_config_get_pre_install_drivers_disk:
 * @config: the install config
 *
 * Returns: The disk on which pre-installation drivers are located, or NULL if
 * its not set using #osinfo_install_config_set_pre_install_drivers_disk().
 */
const gchar *osinfo_install_config_get_pre_install_drivers_disk(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value
            (OSINFO_ENTITY(config),
             OSINFO_INSTALL_CONFIG_PROP_PRE_INSTALL_DRIVERS_DISK);
}

/**
 * osinfo_install_config_set_pre_install_drivers_location:
 * @config: the install config
 * @location: the location
 *
 * Specify the location on which drivers to be installed at the very beginning of
 * installation, are available. Please read documentation on
 * #osinfo_install_config_set_pre_install_drivers_disk() for more information
 * about pre-installation of drivers.
 *
 * Please read documentation on #osinfo_install_config_set_avatar_location() for
 * explanation on the format of @location string.
 */
void osinfo_install_config_set_pre_install_drivers_location(OsinfoInstallConfig *config,
                                                            const gchar *location)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_PRE_INSTALL_DRIVERS_LOCATION,
                            location);
}

/**
 * osinfo_install_config_get_pre_install_drivers_location:
 * @config: the install config
 *
 * Returns: The location on which pre-installation drivers are located, or NULL if
 * its not set using #osinfo_install_config_set_pre_install_drivers_location().
 */
const gchar *osinfo_install_config_get_pre_install_drivers_location(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value
            (OSINFO_ENTITY(config),
             OSINFO_INSTALL_CONFIG_PROP_PRE_INSTALL_DRIVERS_LOCATION);
}

/**
 * osinfo_install_config_set_post_install_drivers_disk:
 * @config: the install config
 * @disk: the target disk
 *
 * Specify the disk on which drivers to be installed at the end of installation,
 * are available.
 *
 * Please read documentation on #osinfo_install_config_set_target_disk() for
 * explanation on the format of @disk string.
 *
 * NOTE: Not every install script supports post-installation of drivers. Use
 * #osinfo_install_script_get_can_post_install_drivers() to find out if an
 * install script supports it.
 */
void osinfo_install_config_set_post_install_drivers_disk(OsinfoInstallConfig *config,
                                                         const gchar *disk)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_POST_INSTALL_DRIVERS_DISK,
                            disk);
}

/**
 * osinfo_install_config_get_post_install_drivers_disk:
 * @config: the install config
 *
 * Returns: The disk on which post-installation drivers are located, or NULL if
 * its not set using #osinfo_install_config_set_post_install_drivers_disk().
 */
const gchar *osinfo_install_config_get_post_install_drivers_disk(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value
            (OSINFO_ENTITY(config),
             OSINFO_INSTALL_CONFIG_PROP_POST_INSTALL_DRIVERS_DISK);
}

/**
 * osinfo_install_config_set_post_install_drivers_location:
 * @config: the install config
 * @location: the location of avatar
 *
 * Specify the disk on which drivers to be installed at the end of installation,
 * are available.
 *
 * Please read documentation on #osinfo_install_config_set_avatar_location() for
 * explanation on the format of @location string.
 *
 * NOTE: Not every install script supports post-installation of drivers. Use
 * #osinfo_install_script_get_can_post_install_drivers() to find out if an
 * install script supports it.
 */
void osinfo_install_config_set_post_install_drivers_location(OsinfoInstallConfig *config,
                                                             const gchar *location)
{
    osinfo_entity_set_param(OSINFO_ENTITY(config),
                            OSINFO_INSTALL_CONFIG_PROP_POST_INSTALL_DRIVERS_LOCATION,
                            location);
}

/**
 * osinfo_install_config_get_post_install_drivers_location:
 * @config: the install config
 *
 * Returns: The disk on which post-installation drivers are located, or NULL if
 * its not set using #osinfo_install_config_set_post_install_drivers_location().
 */
const gchar *osinfo_install_config_get_post_install_drivers_location(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value
            (OSINFO_ENTITY(config),
             OSINFO_INSTALL_CONFIG_PROP_POST_INSTALL_DRIVERS_LOCATION);
}

/**
 * osinfo_install_config_set_driver_signing:
 * @config: the install config
 * @signing: boolean value
 *
 * If a script requires drivers to be signed, this function can be used to
 * disable that security feature. WARNING: Disabling driver signing may very
 * well mean disabling it permanently.
 */
void osinfo_install_config_set_driver_signing(OsinfoInstallConfig *config,
                                              gboolean signing)
{
    osinfo_entity_set_param_boolean(OSINFO_ENTITY(config),
                                    OSINFO_INSTALL_CONFIG_PROP_DRIVER_SIGNING,
                                    signing);
}

/**
 * osinfo_install_config_get_driver_signing:
 * @config: the install config
 *
 * Returns: %TRUE if driver signing is currently enabled, %FALSE otherwise, see
 * #osinfo_install_config_set_driver_signing() for more details about driver
 * signing.
 */
gboolean osinfo_install_config_get_driver_signing(OsinfoInstallConfig *config)
{
    return osinfo_entity_get_param_value_boolean_with_default
            (OSINFO_ENTITY(config),
             OSINFO_INSTALL_CONFIG_PROP_DRIVER_SIGNING,
             TRUE);
}

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
