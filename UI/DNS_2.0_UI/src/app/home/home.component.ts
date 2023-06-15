import { Component, OnInit } from '@angular/core';
import { DnsService } from '../dns.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit {

  constructor(
    public dns: DnsService,
    public router: Router,) { }

  registers:any = [];
  ngOnInit(): void {
    this.loadRegisters()
    
  }

  loadRegisters(){
    this.dns.getAll()
    .subscribe((data:any)=>{
      this.registers = data.documents;
      console.log(this.registers)
    })
  }

  nuevoRegistro(){
    console.log("new")
    this.router.navigateByUrl('/new');
  }

}
