/*
 * libosinfo:
 *
 * Copyright (C) 2009-2012 Red Hat, Inc.
 * Copyright (C) 2012 Fabiano Fidêncio
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
 *   Christophe Fergeau <cfergeau@redhat.com>
 *   Fabiano Fidêncio <fabiano@fidencio.org>
 */

#include <config.h>

#include <osinfo/osinfo.h>
#include <glib/gi18n-lib.h>

G_DEFINE_TYPE (OsinfoInstallConfigParam, osinfo_install_config_param, OSINFO_TYPE_ENTITY);

#define OSINFO_INSTALL_CONFIG_PARAM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), OSINFO_TYPE_INSTALL_CONFIG_PARAM, OsinfoInstallConfigParamPrivate))

/**
 * SECTION:osinfo_install_config_param
 * @short_description: OS install configuration parameters (and its policies)
 * @see_also: #OsinfoInstallScript, #OsinfoInstallConfig
 *
 * #OsinfoInstallConfigParam is an entity for describing all parameters that
 * can be set in an automated installation. It is used to help applications to
 * generate an automated installation script. The actual parameter values
 * for an #OsinfoInstallScript must be set using an #OsinfoInstallConfig
 * object.
 */

struct _OsinfoInstallConfigParamPrivate
{
    OsinfoDatamap *value_map;
};

enum {
    PROP_0,

    PROP_NAME,
    PROP_POLICY,
    PROP_VALUE_MAP
};

static void
osinfo_install_config_param_set_property(GObject *object,
                                         guint property_id,
                                         const GValue *value,
                                         GParamSpec *pspec)
{
    OsinfoInstallConfigParam *config_param =
        OSINFO_INSTALL_CONFIG_PARAM (object);

    switch (property_id) {
    case PROP_NAME:
        osinfo_entity_set_param(OSINFO_ENTITY(config_param),
                                OSINFO_INSTALL_CONFIG_PARAM_PROP_NAME,
                                g_value_get_string(value));
        break;

    case PROP_VALUE_MAP:
        osinfo_install_config_param_set_value_map(config_param,
                                                  g_value_get_object(value));
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
osinfo_install_config_param_get_property(GObject *object,
                                         guint property_id,
                                         GValue *value,
                                         GParamSpec *pspec)
{
    OsinfoInstallConfigParam *config_param =
        OSINFO_INSTALL_CONFIG_PARAM (object);

    switch (property_id) {
    case PROP_NAME:
    {
        const gchar *name;

        name = osinfo_install_config_param_get_name(config_param);
        g_value_set_string(value, name);
        break;
    }
    case PROP_POLICY:
    {
        OsinfoInstallConfigParamPolicy policy;

        policy = osinfo_install_config_param_get_policy(config_param);
        g_value_set_enum(value, policy);
        break;
    }
    case PROP_VALUE_MAP:
        g_value_set_object(value, config_param->priv->value_map);
        break;
    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
osinfo_install_config_param_finalize(GObject *object)
{
    OsinfoInstallConfigParam *config_param;
    config_param = OSINFO_INSTALL_CONFIG_PARAM(object);
    g_clear_object(&config_param->priv->value_map);

    /* Chain up to the parent class */
    G_OBJECT_CLASS (osinfo_install_config_param_parent_class)->finalize (object);
}

/* Init functions */
static void
osinfo_install_config_param_class_init (OsinfoInstallConfigParamClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_klass->set_property = osinfo_install_config_param_set_property;
    g_klass->get_property = osinfo_install_config_param_get_property;

    /**
     * OsinfoInstallConfigParam:name:
     *
     * The name of the configuration parameter.
     **/
    pspec = g_param_spec_string("name",
                                "Name",
                                _("Parameter name"),
                                NULL,
                                G_PARAM_WRITABLE |
                                G_PARAM_READABLE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass,
                                    PROP_NAME,
                                    pspec);
    /**
     * OsinfoInstallConfigParam:policy:
     *
     * The policy of the configuration parameter.
     **/
    pspec = g_param_spec_enum("policy",
                              "Policy",
                              _("Parameter policy"),
                              OSINFO_TYPE_INSTALL_CONFIG_PARAM_POLICY,
                              OSINFO_INSTALL_CONFIG_PARAM_POLICY_OPTIONAL,
                              G_PARAM_READABLE |
                              G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass,
                                    PROP_POLICY,
                                    pspec);
    /**
     * OsinfoInstallConfigParam:value-map:
     *
     * The mapping between generic values and OS-specific values for this
     * configuration parameter
     **/
    pspec = g_param_spec_object("value-map",
                              "Value Mapping",
                              _("Parameter Value Mapping"),
                              OSINFO_TYPE_DATAMAP,
                              G_PARAM_READWRITE |
                              G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(g_klass,
                                    PROP_VALUE_MAP,
                                    pspec);

    g_klass->finalize = osinfo_install_config_param_finalize;
    g_type_class_add_private (klass, sizeof (OsinfoInstallConfigParamPrivate));
}

static void
osinfo_install_config_param_init (OsinfoInstallConfigParam *config_param)
{
    config_param->priv = OSINFO_INSTALL_CONFIG_PARAM_GET_PRIVATE(config_param);
}

/**
 * osinfo_install_config_param_new:
 * @name: the configuration parameter name
 *
 * Construct a new configuration parameter for an #OsinfoInstallScript.
 *
 * Returns: (transfer full): the new configuration parameter
 */
OsinfoInstallConfigParam *osinfo_install_config_param_new(const gchar *name)
{
    return g_object_new(OSINFO_TYPE_INSTALL_CONFIG_PARAM,
                        "id", name,
                        "name", name,
                        NULL);
}

/**
 * osinfo_install_config_param_get_name:
 * @config_param: the configuration parameter
 *
 * Returns: (transfer none): the name of the configuration parameter
 */
const gchar *osinfo_install_config_param_get_name(const OsinfoInstallConfigParam *config_param)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(config_param),
                                         OSINFO_INSTALL_CONFIG_PARAM_PROP_NAME);
}

/**
 * osinfo_install_config_param_get_policy:
 * @config_param: the configuration parameter
 *
 * Returns: (transfer none): the policy of the configuration parameter
 */
OsinfoInstallConfigParamPolicy osinfo_install_config_param_get_policy(const OsinfoInstallConfigParam *config_param)
{
    return osinfo_entity_get_param_value_enum(OSINFO_ENTITY(config_param),
                                              OSINFO_INSTALL_CONFIG_PARAM_PROP_POLICY,
                                              OSINFO_TYPE_INSTALL_CONFIG_PARAM_POLICY,
                                              OSINFO_INSTALL_CONFIG_PARAM_POLICY_OPTIONAL);
}

/**
 * osinfo_install_config_param_is_required:
 * @config_param: the configuration parameter
 *
 * Returns: (transfer none): TRUE if the config_param is required.
 *                           FALSE otherwise.
 */
gboolean osinfo_install_config_param_is_required(const OsinfoInstallConfigParam *config_param)
{
    return (osinfo_install_config_param_get_policy(config_param) ==
            OSINFO_INSTALL_CONFIG_PARAM_POLICY_REQUIRED);
}

/**
 * osinfo_install_config_param_is_optional:
 * @config_param: the configuration parameter
 *
 * Returns: (transfer none): TRUE if the config_param is optional.
 *                           FALSE otherwise.
 */
gboolean osinfo_install_config_param_is_optional(const OsinfoInstallConfigParam *config_param)
{
    return (osinfo_install_config_param_get_policy(config_param) ==
            OSINFO_INSTALL_CONFIG_PARAM_POLICY_OPTIONAL);
}

OsinfoDatamap *osinfo_install_config_param_get_value_map(const OsinfoInstallConfigParam *config_param)
{
    return config_param->priv->value_map;
}

/**
 * osinfo_install_config_param_set_value_map:
 * @config_param: the configuration parameter
 * @datamap: a #OsinfoDatamap to transform values this parameter is set to,
 * or NULL to disable transformations for this parameter
 *
 * After a call to osinfo_install_config_param_set_value_map(), @datamap will
 * be used to transform values set for this parameter to OS-specific
 * values. A NULL @datamap will disable transformations.
 */
void osinfo_install_config_param_set_value_map(OsinfoInstallConfigParam *config_param, OsinfoDatamap *datamap)
{
    g_return_if_fail(OSINFO_IS_INSTALL_CONFIG_PARAM(config_param));

    if (config_param->priv->value_map != NULL)
        g_object_unref(G_OBJECT(config_param->priv->value_map));
    config_param->priv->value_map = g_object_ref(datamap);
}

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */
