import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DnsEditFormComponent } from './dns-edit-form.component';

describe('DnsEditFormComponent', () => {
  let component: DnsEditFormComponent;
  let fixture: ComponentFixture<DnsEditFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DnsEditFormComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DnsEditFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
