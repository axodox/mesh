import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UdpLightSourceComponent } from './udp-light-source.component';

describe('UdpLightSourceComponent', () => {
  let component: UdpLightSourceComponent;
  let fixture: ComponentFixture<UdpLightSourceComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UdpLightSourceComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(UdpLightSourceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
