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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Authors:
 *   Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <osinfo/osinfo.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>

G_DEFINE_TYPE (OsinfoInstallScript, osinfo_install_script, OSINFO_TYPE_ENTITY);

#define OSINFO_INSTALL_SCRIPT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), OSINFO_TYPE_INSTALL_SCRIPT, OsinfoInstallScriptPrivate))

/**
 * SECTION:osinfo_install_script
 * @short_description: OS install scripturation
 * @see_also: #OsinfoInstallScript
 *
 * #OsinfoInstallScript is an object for representing OS
 * install scripturation data. It is used to generate an
 * automated installation script
 */

struct _OsinfoInstallScriptPrivate
{
    gboolean unused;
};

enum {
    PROP_0,

    PROP_TEMPLATE_URI,
    PROP_TEMPLATE_DATA,
    PROP_PROFILE,
    PROP_PRODUCT_KEY_FORMAT,
};

typedef struct _OsinfoInstallScriptGenerateData OsinfoInstallScriptGenerateData;
typedef struct _OsinfoInstallScriptGenerateSyncData OsinfoInstallScriptGenerateSyncData;


static void
osinfo_os_set_property(GObject    *object,
                       guint       property_id,
                       const GValue     *value,
                       GParamSpec *pspec)
{
    OsinfoInstallScript *script = OSINFO_INSTALL_SCRIPT(object);
    const gchar *data;

    switch (property_id) {
    case PROP_TEMPLATE_URI:
        data = g_value_get_string(value);
        if (data)
            osinfo_entity_set_param(OSINFO_ENTITY(script),
                                    OSINFO_INSTALL_SCRIPT_PROP_TEMPLATE_URI,
                                    data);
        break;

    case PROP_TEMPLATE_DATA:
        data = g_value_get_string(value);
        if (data)
            osinfo_entity_set_param(OSINFO_ENTITY(script),
                                    OSINFO_INSTALL_SCRIPT_PROP_TEMPLATE_DATA,
                                    data);
        break;

    case PROP_PROFILE:
        data = g_value_get_string(value);
        if (data)
            osinfo_entity_set_param(OSINFO_ENTITY(script),
                                    OSINFO_INSTALL_SCRIPT_PROP_PROFILE,
                                    data);
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
osinfo_os_get_property(GObject    *object,
                       guint       property_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
    OsinfoInstallScript *script = OSINFO_INSTALL_SCRIPT(object);

    switch (property_id) {
    case PROP_TEMPLATE_URI:
        g_value_set_string(value,
                           osinfo_install_script_get_template_uri(script));
        break;

    case PROP_TEMPLATE_DATA:
        g_value_set_string(value,
                           osinfo_install_script_get_template_data(script));
        break;

    case PROP_PROFILE:
        g_value_set_string(value,
                           osinfo_install_script_get_profile(script));
        break;

    case PROP_PRODUCT_KEY_FORMAT:
        g_value_set_string(value,
                           osinfo_install_script_get_product_key_format(script));
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}



static void
osinfo_install_script_finalize (GObject *object)
{
    /* Chain up to the parent class */
    G_OBJECT_CLASS (osinfo_install_script_parent_class)->finalize (object);
}

/* Init functions */
static void
osinfo_install_script_class_init (OsinfoInstallScriptClass *klass)
{
    GObjectClass *g_klass = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_klass->get_property = osinfo_os_get_property;
    g_klass->set_property = osinfo_os_set_property;
    g_klass->finalize = osinfo_install_script_finalize;

    pspec = g_param_spec_string("template-uri",
                                "TemplateURI",
                                "URI for install script template",
                                NULL /* default value */,
                                G_PARAM_READABLE |
                                G_PARAM_WRITABLE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_NAME |
                                G_PARAM_STATIC_NICK |
                                G_PARAM_STATIC_BLURB);
    g_object_class_install_property(g_klass,
                                    PROP_TEMPLATE_URI,
                                    pspec);

    pspec = g_param_spec_string("template-data",
                                "TemplateData",
                                "Data for install script template",
                                NULL /* default value */,
                                G_PARAM_READABLE |
                                G_PARAM_WRITABLE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_NAME |
                                G_PARAM_STATIC_NICK |
                                G_PARAM_STATIC_BLURB);
    g_object_class_install_property(g_klass,
                                    PROP_TEMPLATE_DATA,
                                    pspec);

    pspec = g_param_spec_string("profile",
                                "Profile",
                                "Install script profile name",
                                NULL /* default value */,
                                G_PARAM_READABLE |
                                G_PARAM_WRITABLE |
                                G_PARAM_CONSTRUCT_ONLY |
                                G_PARAM_STATIC_NAME |
                                G_PARAM_STATIC_NICK |
                                G_PARAM_STATIC_BLURB);
    g_object_class_install_property(g_klass,
                                    PROP_PROFILE,
                                    pspec);

    pspec = g_param_spec_string("product-key-format",
                                "Product Key Format",
                                "Product key format mask",
                                NULL /* default value */,
                                G_PARAM_READABLE |
                                G_PARAM_STATIC_NAME |
                                G_PARAM_STATIC_NICK |
                                G_PARAM_STATIC_BLURB);
    g_object_class_install_property(g_klass,
                                    PROP_PROFILE,
                                    pspec);

    g_type_class_add_private (klass, sizeof (OsinfoInstallScriptPrivate));
}

static void
osinfo_install_script_init (OsinfoInstallScript *list)
{
    OsinfoInstallScriptPrivate *priv;
    list->priv = priv = OSINFO_INSTALL_SCRIPT_GET_PRIVATE(list);

}


OsinfoInstallScript *osinfo_install_script_new(const gchar *id)
{
    return g_object_new(OSINFO_TYPE_INSTALL_SCRIPT,
                        "id", id,
                        NULL);
}

/**
 * osinfo_install_script_new_data:
 *
 * Construct a new install script from stylesheet data
 *
 * Returns: (transfer full): an new install script
 */
OsinfoInstallScript *osinfo_install_script_new_data(const gchar *id,
                                                    const gchar *profile,
                                                    const gchar *templateData)
{
    return g_object_new(OSINFO_TYPE_INSTALL_SCRIPT,
                        "id", id,
                        "profile", profile,
                        "template-data", templateData,
                        NULL);
}


/**
 * osinfo_install_script_new_uri:
 *
 * Construct a new install script from a stylesheet URI
 *
 * Returns: (transfer full): an new install script
 */
OsinfoInstallScript *osinfo_install_script_new_uri(const gchar *id,
                                                   const gchar *profile,
                                                   const gchar *templateUri)
{
    return g_object_new(OSINFO_TYPE_INSTALL_SCRIPT,
                        "id", id,
                        "profile", profile,
                        "template-uri", templateUri,
                        NULL);
}


const gchar *osinfo_install_script_get_template_uri(OsinfoInstallScript *script)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(script),
                                         OSINFO_INSTALL_SCRIPT_PROP_TEMPLATE_URI);
}

const gchar *osinfo_install_script_get_template_data(OsinfoInstallScript *script)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(script),
                                         OSINFO_INSTALL_SCRIPT_PROP_TEMPLATE_DATA);
}

const gchar *osinfo_install_script_get_profile(OsinfoInstallScript *script)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(script),
                                         OSINFO_INSTALL_SCRIPT_PROP_PROFILE);
}

const gchar *osinfo_install_script_get_product_key_format(OsinfoInstallScript *script)
{
    return osinfo_entity_get_param_value(OSINFO_ENTITY(script),
                                         OSINFO_INSTALL_SCRIPT_PROP_PRODUCT_KEY_FORMAT);
}

struct _OsinfoInstallScriptGenerateData {
    GSimpleAsyncResult *res;
    OsinfoOs *os;
    OsinfoInstallConfig *config;
    OsinfoInstallScript *script;
};


static void osinfo_install_script_generate_data_free(OsinfoInstallScriptGenerateData *data)
{
    g_object_unref(data->os);
    g_object_unref(data->config);
    g_object_unref(data->script);
    g_object_unref(data->res);
    g_free(data);
}


static xsltStylesheetPtr osinfo_install_script_load_template(const gchar *uri,
                                                             const gchar *template,
                                                             GError **error)
{
    xsltStylesheetPtr xslt = NULL;
    xmlDocPtr doc = NULL;
    xmlParserCtxtPtr pctxt;

    /* Set up a parser context so we can catch the details of XML errors. */
    pctxt = xmlNewParserCtxt();
    if (!pctxt || !pctxt->sax) {
        g_set_error(error, 0, 0, "%s",
                    "Unable to create XML parser context");
        goto cleanup;
    }

    if (!(doc = xmlCtxtReadDoc(pctxt, BAD_CAST template, uri, NULL,
                               XML_PARSE_NOENT | XML_PARSE_NONET |
                               XML_PARSE_NOWARNING))) {
        g_set_error(error, 0, 0, "%s",
                    "Unable to read XSL template");
        goto cleanup;
    }

    if (!(xslt = xsltParseStylesheetDoc(doc))) {
        g_set_error(error, 0, 0, "%s",
                    "Unable to parse XSL template");
        goto cleanup;
    }

 cleanup:
    xmlFreeParserCtxt(pctxt);
    return xslt;
}

static xmlNodePtr osinfo_install_script_generate_entity_config(OsinfoInstallConfig *config,
                                                               OsinfoEntity *entity,
                                                               const gchar *name,
                                                               GError **error)
{
    xmlNodePtr node = NULL;
    xmlNodePtr data = NULL;
    GList *keys;
    GList *tmp1;

    if (!(node = xmlNewDocNode(NULL, NULL, (xmlChar*)name, NULL))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to create XML node '%s': '%s'",
                    name, err ? err->message : "");
        goto error;
    }

    if (!(data = xmlNewDocNode(NULL, NULL, (const xmlChar*)"id",
                               (const xmlChar*)osinfo_entity_get_id(entity)))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to create XML node 'id': '%s'",
                    err ? err->message : "");
        goto error;
    }
    if (!(xmlAddChild(node, data))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to add XML child '%s'", err ? err->message : "");
        goto error;
    }
    data = NULL;

    tmp1 = keys = osinfo_entity_get_param_keys(entity);
    while (tmp1) {
        GList *values = osinfo_entity_get_param_value_list(entity, tmp1->data);
        GList *tmp2 = values;

        while (tmp2) {
            if (!(data = xmlNewDocNode(NULL, NULL, (const xmlChar*)tmp1->data,
                                       (const xmlChar*)tmp2->data))) {
                xmlErrorPtr err = xmlGetLastError();
                g_set_error(error, 0, 0, "Unable to create XML node '%s': '%s'",
                            (const gchar *)tmp1->data, err ? err->message : "");
                goto error;
            }
            if (!(xmlAddChild(node, data))) {
                xmlErrorPtr err = xmlGetLastError();
                g_set_error(error, 0, 0, "Unable to add XML child '%s'", err ? err->message : "");
                goto error;
            }
            data = NULL;

            tmp2 = tmp2->next;
        }
        g_list_free(values);

        tmp1 = tmp1->next;
    }
    g_list_free(keys);

    return node;

 error:
    xmlFreeNode(data);
    xmlFreeNode(node);
    return NULL;
}


static xmlDocPtr osinfo_install_script_generate_config_xml(OsinfoInstallScript *script,
                                                           OsinfoOs *os,
                                                           OsinfoInstallConfig *config,
                                                           GError **error)
{
    xmlDocPtr doc = xmlNewDoc((xmlChar *)"1.0");
    xmlNodePtr root;
    xmlNodePtr node;

    root = xmlNewDocNode(NULL,
                         NULL,
                         (xmlChar*)"install-script-config",
                         NULL);
    xmlDocSetRootElement(doc, root);

    if (!(node = osinfo_install_script_generate_entity_config(config,
                                                              OSINFO_ENTITY(script),
                                                              "script",
                                                              error)))
        goto error;
    if (!(xmlAddChild(root, node))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to set XML root '%s'", err ? err->message : "");
        goto error;
    }

    if (!(node = osinfo_install_script_generate_entity_config(config,
                                                              OSINFO_ENTITY(os),
                                                              "os",
                                                              error)))
        goto error;
    if (!(xmlAddChild(root, node))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to set XML root '%s'", err ? err->message : "");
        goto error;
    }

    if (!(node = osinfo_install_script_generate_entity_config(config,
                                                              OSINFO_ENTITY(config),
                                                              "config",
                                                              error)))
        goto error;
    if (!(xmlAddChild(root, node))) {
        xmlErrorPtr err = xmlGetLastError();
        g_set_error(error, 0, 0, "Unable to set XML root '%s'", err ? err->message : "");
        goto error;
    }

    return doc;

 error:
    xmlFreeDoc(doc);
    doc = NULL;
    return NULL;
}

static gchar *osinfo_install_script_apply_xslt(xsltStylesheetPtr ss,
                                               xmlDocPtr doc,
                                               GError **error)
{
    xsltTransformContextPtr ctxt;
    gchar *ret;
    xmlDocPtr docOut = NULL;
    int len;

    if (!(ctxt = xsltNewTransformContext(ss, doc))) {
        g_set_error(error, 0, 0, "%s", "Unable to create XSL transform context");
        goto cleanup;
    }

    if (!(docOut = xsltApplyStylesheetUser(ss, doc, NULL, NULL, NULL, ctxt))) {
        g_set_error(error, 0, 0, "%s", "Unable to apply XSL transform context");
        goto cleanup;
    }

    if (xsltSaveResultToString((xmlChar **)&ret, &len, docOut, ss) < 0) {
        g_set_error(error, 0, 0, "%s", "Unable to convert XSL output to string");
        goto cleanup;
    }

 cleanup:
    xmlFreeDoc(docOut);
    xsltFreeTransformContext(ctxt);
    return ret;
}


static gboolean osinfo_install_script_apply_template(OsinfoInstallScript *script,
                                                     OsinfoOs *os,
                                                     const gchar *templateUri,
                                                     const gchar *template,
                                                     gchar **result,
                                                     OsinfoInstallConfig *config,
                                                     GError **error)
{
    gboolean ret = FALSE;
    xsltStylesheetPtr templateXsl = osinfo_install_script_load_template(templateUri, template, error);
    xmlDocPtr configXml = osinfo_install_script_generate_config_xml(script, os, config, error);

    if (!templateXsl || !configXml)
        goto cleanup;

    if (!(*result = osinfo_install_script_apply_xslt(templateXsl, configXml, error)))
        goto cleanup;

    ret = TRUE;

 cleanup:
    xsltFreeStylesheet(templateXsl);
    xmlFreeDoc(configXml);
    return ret;
}


static void osinfo_install_script_template_loaded(GObject *src,
                                                  GAsyncResult *res,
                                                  gpointer user_data)
{
    OsinfoInstallScriptGenerateData *data = user_data;
    GError *error = NULL;
    gchar *input = NULL;
    gchar *output = NULL;
    gsize length = 0;
    GFile *file = G_FILE(src);
    gchar *uri = g_file_get_uri(file);

    if (!g_file_load_contents_finish(file,
                                     res,
                                     &input,
                                     &length,
                                     NULL,
                                     &error)) {
        g_prefix_error(&error, "Failed to load script template %s: ", uri);
        g_simple_async_result_take_error(data->res, error);
        goto cleanup;
    }


    if (!osinfo_install_script_apply_template(data->script,
                                              data->os,
                                              uri,
                                              input,
                                              &output,
                                              data->config,
                                              &error)) {
        g_prefix_error(&error, "Failed to apply script template %s: ", uri);
        g_simple_async_result_take_error(data->res, error);
        goto cleanup;
    }

    g_simple_async_result_set_op_res_gpointer(data->res,
                                              output, NULL);

 cleanup:
    g_simple_async_result_complete(data->res);
    osinfo_install_script_generate_data_free(data);
    g_free(uri);
}


void osinfo_install_script_generate_async(OsinfoInstallScript *script,
                                          OsinfoOs *os,
                                          OsinfoInstallConfig *config,
                                          GCancellable *cancellable,
                                          GAsyncReadyCallback callback,
                                          gpointer user_data)
{
    OsinfoInstallScriptGenerateData *data = g_new0(OsinfoInstallScriptGenerateData, 1);
    const gchar *templateData = osinfo_install_script_get_template_data(script);
    const gchar *templateUri = osinfo_install_script_get_template_uri(script);

    data->os = g_object_ref(os);
    data->config = g_object_ref(config);
    data->script = g_object_ref(script);
    data->res = g_simple_async_result_new(G_OBJECT(script),
                                          callback,
                                          user_data,
                                          osinfo_install_script_generate_async);

    if (templateData) {
        GError *error = NULL;
        gchar *output;
        if (!osinfo_install_script_apply_template(script,
                                                  os,
                                                  "<data>",
                                                  templateData,
                                                  &output,
                                                  data->config,
                                                  &error)) {
            g_prefix_error(&error, "%s", "Failed to apply script template: ");
            g_simple_async_result_take_error(data->res, error);
            g_simple_async_result_complete(data->res);
            osinfo_install_script_generate_data_free(data);
            return;
        }
        g_simple_async_result_set_op_res_gpointer(data->res,
                                                  output, NULL);
        g_simple_async_result_complete_in_idle(data->res);
        osinfo_install_script_generate_data_free(data);
    } else {
        GFile *file = g_file_new_for_uri(templateUri);

        g_file_load_contents_async(file,
                                   cancellable,
                                   osinfo_install_script_template_loaded,
                                   data);
    }
}

gchar *osinfo_install_script_generate_finish(OsinfoInstallScript *script,
                                             GAsyncResult *res,
                                             GError **error)
{
    GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(res);

    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    if (g_simple_async_result_propagate_error(simple, error))
        return FALSE;

    return g_simple_async_result_get_op_res_gpointer(simple);
}


struct _OsinfoInstallScriptGenerateSyncData {
    GMainLoop *loop;
    GError *error;
    gchar *output;
};

static void osinfo_install_script_generate_done(GObject *src,
                                                GAsyncResult *res,
                                                gpointer user_data)
{
    OsinfoInstallScriptGenerateSyncData *data = user_data;

    data->output =
        osinfo_install_script_generate_finish(OSINFO_INSTALL_SCRIPT(src),
                                              res,
                                              &data->error);
    g_main_loop_quit(data->loop);
}


gchar *osinfo_install_script_generate(OsinfoInstallScript *script,
                                      OsinfoOs *os,
                                      OsinfoInstallConfig *config,
                                      GCancellable *cancellable,
                                      GError **error)
{
    GMainLoop *loop = g_main_loop_new(g_main_context_get_thread_default(),
                                      TRUE);
    OsinfoInstallScriptGenerateSyncData data = {
        loop, NULL, NULL
    };

    osinfo_install_script_generate_async(script,
                                         os,
                                         config,
                                         cancellable,
                                         osinfo_install_script_generate_done,
                                         &data);

    if (g_main_loop_is_running(loop))
        g_main_loop_run(loop);

    if (data.error)
        g_propagate_error(error, data.error);

    g_main_loop_unref(loop);

    return data.output;
}


/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 */