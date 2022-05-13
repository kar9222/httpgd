
import { Httpgd } from 'httpgd';
import { HttpgdPlotsResponse } from 'httpgd/lib/types';
import { ExportView } from './views/exportView';
import { OverlayView } from './views/overlayView';
import { PlotView } from './views/plotView';

export class HttpgdViewer {
    static readonly COOLDOWN_DEVICE_INACTIVE: number = 1000;

    public httpgd: Httpgd;
    public plotView?: PlotView;
    public overlayView?: OverlayView;
    public exportView?: ExportView;
    public sidebarHidden: boolean = false;

    private deviceInactiveDelayed?: ReturnType<typeof setTimeout>;

    public onDeviceActiveChange?: (deviceActive: boolean) => void;
    public onDisconnectedChange?: (disconnected: boolean) => void;
    public onIndexStringChange?: (indexString: string) => void;
    public onZoomStringChange?: (zoomString: string) => void;

    public constructor(host: string, token?: string, allowWebsockets?: boolean, sidebarHidden?: boolean) {
        this.httpgd = new Httpgd(host, token, allowWebsockets);

        if (sidebarHidden) { this.sidebarHidden = true; }

        this.httpgd.onPlotsChanged((newState) => this.plotsChanged(newState));
        this.httpgd.onConnectionChanged((newState) => this.connectionChanged(newState));
        this.httpgd.onDeviceActiveChanged((newState) => this.deviceActiveChanged(newState));
    }

    private plotsChanged(newState: HttpgdPlotsResponse): void {
        this.plotView?.updatePlots(newState);
        this.plotView?.update();
    }

    private connectionChanged(newState: boolean): void {
        if (newState) {
            this.overlayView?.show(OverlayView.TEXT_CONNECTION_LOST);
        } else {
            this.overlayView?.hide();
        }
    }

    private deviceActiveChanged(active: boolean): void {
        if (this.deviceInactiveDelayed) {
            clearTimeout(this.deviceInactiveDelayed);
        }
        if (!active) {
            this.deviceInactiveDelayed = setTimeout(
                () => this.overlayView?.show(OverlayView.TEXT_DEVICE_INACTIVE),
                HttpgdViewer.COOLDOWN_DEVICE_INACTIVE);
        } else {
            this.overlayView?.hide();
        }
    }

    public init(): void {

        this.plotView = new PlotView(this, this.sidebarHidden);
        this.overlayView = new OverlayView(this);
        this.exportView = new ExportView(this);
        
        this.httpgd.connect().then(() => {
            console.log('Connected to httpgd ' + this.httpgd.getInfo().version.httpgd);
            this.exportView.initRenderers();
        });

        // Javascript keycode e.g. keycode for 'a' is 65. See [Keycode](https://keycode.info/). Example:
        // {
        // altKey: true,  // Use alt key. Simply remove it if alt key isn't needed
        // keys: [68],    // The key alone is `d`. Combined key is m-d
        // f: () => this.plotView.clearPlots(),
        // },
        this.plotView.toolbar.registerActions([
            {
                keys: [37, 40, 75],  // left, down, k
                f: () => this.plotView.prevPage(),
                id: "tb-left",
            },
            {
                keys: [39, 38, 74],  // right, up, j
                f: () => this.plotView.nextPage(),
                id: "tb-right",
            },
            {
                keys: [78],  // n
                f: () => this.plotView.newestPage(),
                id: "tb-pnum",
            },
            {
                keys: [187],  // =
                f: () => this.plotView.zoomIn(),
                id: "tb-plus",
            },
            {
                keys: [189],  // -
                f: () => this.plotView.zoomOut(),
                id: "tb-minus",
            },
            {
                keys: [48],  // 0
                f: () => this.plotView.zoomReset(),
                id: "tb-zlvl",
            },
            {
                id: "tb-clear",
                altKey: true,
                keys: [89],  // m-x but AHKREMAP to c-d due to c-d is already mapped for browser
                f: () => this.plotView.clearPlots(),
            },
            {
                id: "tb-remove",
                keys: [46, 68],  // delete, d
                f: () => this.plotView.removePlot(),
            },
            {
                id: "tb-save-svg",
                keys: [83],  // s
                f: () => this.plotView.downloadSVG(),
            },
            {
                id: "tb-save-png",
                keys: [80],  // p
                f: () => this.plotView.downloadPNG(),
            },
            {
                id: "tb-copy-png",
                keys: [67],  // c
                f: () => this.plotView.copyPNG(),
            },
            {
                id: "tb-history",
                keys: [72],  // h
                f: () => this.plotView.sidebar.toggle(),
            },
            {
                id: "tb-export",
                keys: [69],  // e
                f: () => this.exportView.show(),
            },
        ]);
    }
}
