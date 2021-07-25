import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RainbowLightSourceComponent } from './rainbow-light-source.component';

describe('RainbowLightSourceComponent', () => {
  let component: RainbowLightSourceComponent;
  let fixture: ComponentFixture<RainbowLightSourceComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ RainbowLightSourceComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(RainbowLightSourceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
