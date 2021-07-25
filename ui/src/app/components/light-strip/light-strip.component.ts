import { ChangeDetectionStrategy, ChangeDetectorRef, Component, OnInit } from '@angular/core';
import { LightSourceSettings, LightSourceType, NoneSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-light-strip',
  templateUrl: './light-strip.component.html',
  styleUrls: ['./light-strip.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class LightStripComponent implements OnInit {

  source: LightSourceSettings | undefined;

  constructor(
    private readonly changeDetector: ChangeDetectorRef,
    private readonly lightStripService : LightStripService
  ) { 
    
  }

  async ngOnInit() {
    this.source = await this.lightStripService.getLightSourceSettings().toPromise();
    this.changeDetector.markForCheck();
  }

  async onSourceChange(event: any) {
    let sourceSettings = new NoneSourceSettings();
    sourceSettings.$type = event.target.value;
    await this.lightStripService.setLightSourceSettings(sourceSettings).toPromise();
  }
}
