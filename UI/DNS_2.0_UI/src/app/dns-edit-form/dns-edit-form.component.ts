import { Component, OnInit } from '@angular/core';
import { DnsService } from '../dns.service';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-dns-edit-form',
  templateUrl: './dns-edit-form.component.html',
  styleUrls: ['./dns-edit-form.component.css']
})
export class DnsEditFormComponent implements OnInit {

  constructor(
    public dns: DnsService,
    public router: Router,
    private route: ActivatedRoute
  ) { }

  servers:string[] = []
  serversWeight:any[] = []
  type:string = "single";
  url:string = "";
  server:string = "0.0.0.0";
  id:any = ""
  

  ngOnInit(): void {
    this.id = this.route.snapshot.paramMap.get('id');
    this.getData()
  }

  getData(){
    this.dns.getById(this.id).subscribe( (data:any) =>{
      console.log(data)
      this.type = data.document.type;
      if(this.type === "single"){
        this.url = this.id;
        this.server = data.document.server

      }
      if(this.type === "multi"){
        this.url = this.id;
        this.servers = data.document.servers
      }
      if(this.type === "weight"){
        this.url = this.id;
        this.serversWeight = data.document.servers
      }
    })
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
    this.serversWeight.push(["0.0.0.0", 0])
  }

  updateRegister(){
    const register = this.getRegisterJson();
    console.log(register)
    this.dns.update(register).subscribe((data:any) =>{
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
