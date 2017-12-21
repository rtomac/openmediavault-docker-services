// require("js/omv/WorkspaceManager.js")
// require("js/omv/workspace/form/Panel.js")

Ext.define("OMV.module.admin.service.duplicatidocker.Settings", {
    extend : "OMV.workspace.form.Panel",

    rpcService   : "DuplicatiDocker",
    rpcGetMethod : "getSettings",
    rpcSetMethod : "setSettings",

    constructor: function() {
        var me = this;
        me.callParent(arguments);
        me.on('load', this.onLoadValues);
    },

    getButtonItems : function() {
        var me = this;
        var items = me.callParent(arguments);
        items.push({
            id       : me.getId() + "-openweb",
            xtype    : "button",
            text     : _("Open Web Interface"),
            icon     : "images/duplicati.png",
            iconCls  : Ext.baseCSSPrefix + "btn-icon-16x16",
            disabled : true,
            scope    : me,
            handler  : Ext.Function.bind(me.onOpenWebButton, me, [ me ])
        });
        return items;
    },

    getFormItems : function () {
        var id = this.getId();
        return [{
            id            : id + "-status",
            xtype         : "panel",
            bodyPadding   : 10
        },{
            xtype         : "fieldset",
            title         : _("General settings"),
            fieldDefaults : {
                labelSeparator : ""
            },
            items : [{
                xtype      : 'textfield',
                name       : 'container-name',
                fieldLabel : _('Container name'),
                allowBlank : false,
                value      : 'duplicati',
                plugins    : [{
                    ptype : 'fieldinfo',
                    text  : _('Name of the Docker container running Duplicati. Allows service to report its status in services dashboard.')
                }]
            },{
                xtype         : "numberfield",
                name          : "port",
                fieldLabel    : _("Port"),
                vtype         : "port",
                minValue      : 1,
                maxValue      : 65535,
                allowDecimals : false,
                allowBlank    : false,
                value         : 8200,
                plugins    : [{
                    ptype : 'fieldinfo',
                    text  : _('Port on which the Docker container is serving the Duplicati UI.')
                }]
            }]
        }];
    },

    onLoadValues: function(me, response) {
        var statusPanel = me.queryById(me.getId() + "-status");
        statusPanel.setHtml(response['container-status']);
        me.setButtonDisabled("openweb", !response["container-running"]);
    },

    onOpenWebButton : function() {
        var me = this;
        window.open("http://" + window.location.hostname + ":" + me.getForm().findField("port").getValue(), "_blank");
    }
});

OMV.WorkspaceManager.registerPanel({
    id        : "settings",
    path      : "/service/duplicatidocker",
    text      : _("Settings"),
    position  : 10,
    className : "OMV.module.admin.service.duplicatidocker.Settings"
});
