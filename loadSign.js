const path = require('node:path');
const os = require('node:os');

const exePath = path.dirname(process.execPath);

let QQWrapper, version, appid, qua;

if (os.platform() === "win32") {
    const versionConfig = require(path.join(exePath, "resources/app/versions/config.json"));
    version = versionConfig.curVersion;
    QQWrapper = require(path.join(exePath, "resources/app/versions", version, "wrapper.node"));
    appid = "537226655"; // 9.9.12-25234
    qua = `V1_WIN_NQ_${version.replace("-", "_")}_GW_B`;
} else {
    const qqPkgInfo = require(path.join(exePath, "resources/app/package.json"));
    QQWrapper = require(path.join(exePath, "resources/app/wrapper.node"));
    version = qqPkgInfo.version;
    appid = "537226441";
    qua = qqPkgInfo.qua;
}

class GlobalAdapter {
    onLog(...args) {
    }
    onGetSrvCalTime(...args) {
    }
    onShowErrUITips(...args) {
    }
    fixPicImgType(...args) {
    }
    getAppSetting(...args) {
    }
    onInstallFinished(...args) {
    }
    onUpdateGeneralFlag(...args) {
    }
    onGetOfflineMsg(...args) {
    }
}

const dataPathGlobal = "/root"

const engine = new QQWrapper.NodeIQQNTWrapperEngine();
engine.initWithDeskTopConfig({
    base_path_prefix: "",
    platform_type: 3,
    app_type: 4,
    app_version: version,
    os_version: os.release(),
    use_xlog: true,
    qua: qua,
    global_path_config: {
        desktopGlobalPath: dataPathGlobal
    },
    thumb_config: { maxSide: 324, minSide: 48, longLimit: 6, density: 2 }
}, new QQWrapper.NodeIGlobalAdapter(new GlobalAdapter()));

const loginService = new QQWrapper.NodeIKernelLoginService();
loginService.initConfig({
    machineId: "",
    appid: appid,
    platVer: os.release(),
    commonPath: dataPathGlobal,
    clientVer: version,
    hostName: os.hostname()
});