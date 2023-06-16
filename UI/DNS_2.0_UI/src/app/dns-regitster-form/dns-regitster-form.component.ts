import { Component, OnInit } from '@angular/core';
import { DnsService } from '../dns.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-dns-regitster-form',
  templateUrl: './dns-regitster-form.component.html',
  styleUrls: ['./dns-regitster-form.component.css']
})
export class DNSRegitsterFormComponent implements OnInit {

  constructor(
    public dns: DnsService,
    public router: Router,
  ) { }

  servers:string[] = []
  serversWeight:any[] = []
  type:string = "single";
  url:string = "";
  server:string = "0.0.0.0";

  ngOnInit(): void {
    
  }

  removeServer(index:number){
    this.servers.splice(index,1);
  }

  addServer(){
    this.servers.push("0.0.0.0")
  }

  removeServerWeight(index:number){
    this.serversWeight.splice(index,1);
  }

  addServerWeight(){
    this.serversWeight.push({"server":"0.0.0.0", "w":0})
  }

  AddRegister(){
    const register = this.getRegisterJson();
    console.log(register)
    this.dns.new(register).subscribe((data:any) =>{
      console.log(data)
      alert(data.message)
      if(data.success){
        this.router.navigate(['/']);
      }
    })
  }

  
  getRegisterJson(){
    var register = {}
    if(this.type === "single"){
      register = {
        "url": this.url,
        "document" : {
            
            "type": this.type,
            "server": this.server
            
        }
      }
    }
    else if(this.type === "multi"){
      register = {
        "url": this.url,
        "document" : {
            
            "type": this.type,
            "servers": this.servers
          }       
        }
    }

    else if(this.type === "weight"){
      register = {
        "url": this.url,
        "document" : {
            
            "type": this.type,
            "servers": this.serversWeight
          }       
        }
    }


    else if(this.type === "geo"){
      register = {
        "url": this.url,
        "document" : {
            
            "type": this.type,
            "Dummy":"www.dummy.com"
          }       
        }
    }
    return register
  }
}
