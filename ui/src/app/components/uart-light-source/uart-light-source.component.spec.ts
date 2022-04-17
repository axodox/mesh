import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UartLightSourceComponent } from './uart-light-source.component';

describe('UartLightSourceComponent', () => {
  let component: UartLightSourceComponent;
  let fixture: ComponentFixture<UartLightSourceComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UartLightSourceComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(UartLightSourceComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
